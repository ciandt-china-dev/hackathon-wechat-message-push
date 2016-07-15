# mysql root password
$mysqlpw = "3.1415926"

# default executable path
Exec {
    path => ["/usr/bin", "/bin", "/usr/sbin", "/sbin", "/usr/local/bin", "/usr/local/sbin"],
}

# update apt sources list
file { "/etc/apt/sources.list":
    source => "puppet:///modules/apt/sources.list",
}

# ensure local apt cache index is up to date at beginning
exec { "apt-get update":
    require => File["/etc/apt/sources.list"],
}

# silence puppet and vagrant annoyance about the puppet group
group { "puppet":
    ensure => "present",
}

include tools, wechat-push

class tools {
    apt::install{ [ "vim", "curl", "htop" ]: }
}

class wechat-push {
    include mysql, memcache, apache-fastcgi, phpmyadmin

    apt::install { ["drush"]: }

    file { "/var/www/wechat-push":
        ensure => directory,
        require => Package["apache2"],
    }

    file { "/etc/php5/fpm/pool.d/www.conf":
        ensure => absent,
        notify => Service["php5-fpm"],
        require => Package["php5-fpm"],
    }

    file { "/etc/php5/fpm/pool.d/wechat-push.conf":
        source => "puppet:///modules/php-fpm/pool.d/sample.conf",
        backup => '.original',
        notify => Service["php5-fpm"],
        require => Package["php5-fpm"],
    }

    # create wechat-push database
    exec { "create-mysql-database":
        unless => "echo 'SHOW DATABASES' | mysql -uroot -p$mysqlpw | grep wechat-push",
        command => "echo 'CREATE DATABASE wechat-push' | mysql -uroot -p$mysqlpw",
        require => Exec["set-mysql-password"],
    }

    apache::vhost { ["wechat-push"]: }
}

define apt::install() {
    package { "${name}":
        ensure => present,
        require => Exec["apt-get update"],
    }
}
