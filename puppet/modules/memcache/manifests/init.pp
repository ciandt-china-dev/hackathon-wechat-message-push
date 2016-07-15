class memcache {
    apt::install{ [ "memcached" ]: }

    # start the memcached service
    service { "memcached":
        ensure => running,
        require => Package["memcached"],
    }
}
