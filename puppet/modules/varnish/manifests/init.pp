class varnish {
    apt::install { [ "apt-transport-https" ]: }

    exec { "Retrieve Varnish GPG Key":
        command => "curl https://repo.varnish-cache.org/ubuntu/GPG-key.txt | apt-key add -",
        require => Package["apt-transport-https"],
    }

    exec { "Add Varnish APT Sources":
        command => "echo 'deb https://repo.varnish-cache.org/ubuntu/ trusty varnish-4.0' >> /etc/apt/sources.list.d/varnish-cache.list",
        require => Exec["Retrieve Varnish GPG Key"],
    }

    exec { "Update Varnish APT Sources":
        command => "apt-get update",
        require => Exec["Add Varnish APT Sources"],
    }

    package { "varnish":
        ensure => present,
        require => Exec["Update Varnish APT Sources"],
    }

    # start docker service
    service { "varnish":
        ensure => running,
        require => Package["varnish"],
    }

    file { "/etc/default/varnish":
        source => "puppet:///modules/varnish/etc/default/varnish",
        backup => '.original',
        notify => Service["varnish"],
        require => Package["varnish"],
    }
}
