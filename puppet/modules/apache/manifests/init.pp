class apache {
    apt::install{ ["apache2"]: }

    # start the apache2 service
    service { "apache2":
        ensure => running,
        require => Package["apache2"],
    }

    apache::module { ['ssl.load', 'rewrite.load']: }

    file { "/etc/apache2/sites-enabled/000-default.conf":
        ensure => absent,
        notify => Service["apache2"],
        require => Package["apache2"],
    }
}

define apache::conf() {
    file { "/etc/apache2/conf-enabled/${name}":
        ensure => link,
        target => "../conf-available/${name}",
        notify => Service["apache2"],
        require => [
            File["/etc/apache2/conf-available/${name}"],
            Package["apache2"],
        ]
    }
}

define apache::vhost() {
    file {
        "/etc/apache2/sites-enabled/${name}.conf":
            ensure => link,
            target => "../sites-available/${name}.conf",
            notify => Service["apache2"],
            require => File["/etc/apache2/sites-available/${name}.conf"];

        "/etc/apache2/sites-available/${name}.conf":
            source => "puppet:///modules/apache/vhosts/sample.conf",
            require => Package["apache2"];

        "/etc/apache2/sites-enabled/${name}-ssl.conf":
            ensure => link,
            target => "../sites-available/${name}-ssl.conf",
            notify => Service["apache2"],
            require => File["/etc/apache2/sites-available/${name}-ssl.conf"];

        "/etc/apache2/sites-available/${name}-ssl.conf":
            source => "puppet:///modules/apache/vhosts/sample-ssl.conf",
            require => Package["apache2"];
    }
}

define apache::module() {
    file { "/etc/apache2/mods-enabled/${name}":
        ensure => link,
        target => "../mods-available/${name}",
        notify => Service['apache2'],
        require => Package['apache2'],
    }
}
