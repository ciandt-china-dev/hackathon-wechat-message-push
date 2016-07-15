class mysql {
    apt::install{ ["mysql-server"]: }

    # start mysql service
    service { "mysql":
        ensure => running,
        require => Package["mysql-server"],
    }

    # set mysql root password
    exec { "set-mysql-password":
        unless => "mysqladmin -uroot -p$mysqlpw status",
        command => "mysqladmin -uroot password $mysqlpw",
        require => Service["mysql"],
    }
}
