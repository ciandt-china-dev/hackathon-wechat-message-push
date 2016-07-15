class docker {
    exec { "add-gpg-key":
        command => "apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D",
    }

    file { "/etc/apt/sources.list.d/docker.list":
        source => "puppet:///modules/docker/etc/apt/sources.list.d/docker.list",
    }

    exec { "update docker apt sources":
        command => "apt-get update",
        require => [
            Exec["add-gpg-key"],
            File["/etc/apt/sources.list.d/docker.list"],
        ],
    }

    exec { "apt-get purge lxc-docker":
        require => Exec["update docker apt sources"],
    }
    
    exec { "apt-cache policy docker-engine":
        require => Exec["apt-get purge lxc-docker"],
    }

    exec { "Install linux-image-extra kernel package":
        command => "apt-get install -y linux-image-extra-$(uname -r)",
    }

    package { "docker-engine":
        ensure => present,
        require => [
            Exec["apt-cache policy docker-engine"],
        ],
    }

    exec { "Configure docker accelerator":
        command => 'echo "DOCKER_OPTS=\"\$DOCKER_OPTS --registry-mirror=http://houchaohann.m.alauda.cn\"" >> /etc/default/docker',
        notify => Service["docker"],
    }

    service { "docker":
        ensure => running,
        require => Package["docker-engine"],
    }
}
