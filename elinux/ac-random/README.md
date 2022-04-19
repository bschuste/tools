
Place rngd.service under:
/lib/systemd/system/

systemctl enable hotplug
cd /lib/systemd/system/sysinit.target.wants/
ln -s systemd-hotplug.service ../systemd-hotplug.service
