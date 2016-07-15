class phpmyadmin {
    apt::install { ["phpmyadmin"]: }

    file { "/etc/apache2/conf-available/phpmyadmin.conf":
        ensure => link,
        target => "/etc/phpmyadmin/apache.conf",
    }

    exec { "import-phpmyadmin-database":
        require => [
            Package["phpmyadmin"],
            Exec["set-mysql-password"]
        ],
        command => "zcat /usr/share/doc/phpmyadmin/examples/create_tables.sql.gz | mysql -uroot -p$mysqlpw",
    }

    exec { "configure-phpmyadmin-controlled-users":
        require => Exec["import-phpmyadmin-database"],
        command => "mysql -uroot -p$mysqlpw -e \"GRANT ALL ON \\`phpmyadmin\\`.* TO 'phpmyadmin'@'localhost' IDENTIFIED BY '$mysqlpw';\"",
    }

    exec { "correct-phpmyadmin-configured-table-names":
        unless => "grep -q 'pma__' /etc/phpmyadmin/config.inc.php",
        require => Package["phpmyadmin"],
        command => "sed -i 's/pma_/pma__/g' /etc/phpmyadmin/config.inc.php",
    }

    file { "/etc/phpmyadmin/config-db.php":
        source => "puppet:///modules/phpmyadmin/config-db.php",
        require => Package["phpmyadmin"],
    }

    apache::conf { ['phpmyadmin.conf']: }
}
