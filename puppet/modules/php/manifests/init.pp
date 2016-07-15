class php {
    apt::install{ [ "php5", "php5-gd", "php5-curl", "php5-mysql", "php5-xdebug", "php5-memcached" ]: }

    exec { "php5enmod mcrypt":
        require => Package["php5"],
    }

    file { "/etc/php5/mods-available/xdebug.ini":
        source => "puppet:///modules/php/xdebug.ini",
        backup => '.original',
        require => Package["php5-xdebug"],
    }
}
