# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.define "default", primary: true do |default|
    default.vm.box = "ubuntu/trusty64"

    default.vm.box_check_update = false

    default.vm.network "private_network", ip: "10.0.0.254"

    #default.vm.synced_folder ".", "/vagrant",
    #  mount_options: ["dmode=775,fmode=664"]

    default.vm.synced_folder ".", "/vagrant", type: "nfs"

    #default.vm.synced_folder ".", "/var/www/wechat-push",
    #  mount_options: ["dmode=775,fmode=664"]

    default.vm.synced_folder ".", "/var/www/wechat-push", type: "nfs"

    default.vm.provision "puppet" do |puppet|
      puppet.manifests_path = "puppet/manifests"
      puppet.manifest_file = 'default.pp'
      puppet.module_path = 'puppet/modules'
      puppet.options = ['--verbose']
    end
  end

  config.vm.define "docker", autostart: false do |docker|
    docker.vm.provider "docker" do |dk|
      dk.name = "cit_wechat-push"
      dk.image = "cit/wechat-push"
      dk.has_ssh = true
    end

    docker.vm.synced_folder ".", "/vagrant",
      mount_options: ["dmode=775,fmode=664"]

    docker.vm.synced_folder ".", "/var/www/wechat-push",
      mount_options: ["dmode=775,fmode=664"]
  end

  config.vm.define "docker_build", autostart: false do |docker|
    docker.vm.provider "docker" do |dk|
      dk.name = "cit_wechat-push"
      dk.has_ssh = true
      dk.build_dir = "."
      dk.build_args = ["-t=cit/wechat-push"]
    end

    docker.vm.provision "puppet" do |puppet|
      puppet.manifests_path = "puppet/manifests"
      puppet.manifest_file = 'default.pp'
      puppet.module_path = 'puppet/modules'
      puppet.options = ['--verbose']
    end

    docker.vm.synced_folder ".", "/vagrant",
      mount_options: ["dmode=775,fmode=664"]

    docker.vm.synced_folder ".", "/var/www/wechat-push",
      mount_options: ["dmode=775,fmode=664"]
  end

  if Vagrant.has_plugin?("vagrant-hostsupdater")
    config.hostsupdater.aliases = [
      "wechat-push.vagrant",
    ]
  end
end
