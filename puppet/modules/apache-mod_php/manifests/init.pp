class apache-mod_php {
    include php, apache

    file { "/etc/php5/apache2/php.ini":
        source => "puppet:///modules/php/php.ini",
        backup => '.original',
        notify => Service["apache2"],
        require => Package["php5"],
    }

    # change apache user and group to vagrant to avoid privilegs issues
    exec { "echo 'export APACHE_RUN_USER=vagrant' >> /etc/apache2/envvars; echo 'export APACHE_RUN_GROUP=vagrant' >> /etc/apache2/envvars":
        unless => "grep -q 'APACHE_RUN_USER=vagrant' /etc/apache2/envvars",
        notify => Service["apache2"],
        require => Package["apache2"],
    }
}
