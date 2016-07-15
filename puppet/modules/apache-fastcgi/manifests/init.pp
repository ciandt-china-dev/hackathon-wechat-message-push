class apache-fastcgi {
    include apache, php-fpm

    file { "/etc/php5/fpm/php.ini":
        source => "puppet:///modules/php/php.ini",
        backup => '.original',
        require => Package["php5-fpm"],
    }

    apt::install{ ["libapache2-mod-fastcgi"]: }

    exec { "a2dismod php5":
        require => Package["apache2"],
    }

    apache::module { ['actions.conf', 'actions.load', 'fastcgi.conf', 'fastcgi.load']: }
}
