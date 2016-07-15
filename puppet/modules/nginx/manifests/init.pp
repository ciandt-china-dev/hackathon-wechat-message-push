class nginx {
    apt::install { [ "nginx" ]: }

    service { "nginx":
        ensure => running,
        require => Package["nginx"],
    }
}

