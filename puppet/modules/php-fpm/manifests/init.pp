class php-fpm {
    include php

    apt::install{ [ "php5-fpm" ]: }

    service { "php5-fpm":
        ensure => running,
    }
}
