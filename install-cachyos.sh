#!/usr/bin/env bash

set -Eeuo pipefail

REPO_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)

if ((EUID == 0)); then
	printf 'Bu scripti sudo ile degil normal kullanici olarak calistirin; gereken adimlarda kendisi sudo kullanir.\n' >&2
	exit 1
fi

on_error() {
	printf 'Kurulum %d. satirda durdu. Once yukaridaki hatayi duzeltin; script tekrar guvenle calistirilabilir.\n' "$1" >&2
}
trap 'on_error $LINENO' ERR

if ! command -v pacman >/dev/null 2>&1; then
	printf 'Bu kurulum yalnizca CachyOS ve Arch tabanli sistemleri destekler.\n' >&2
	exit 1
fi

if [[ -r /etc/os-release ]]; then
	# shellcheck disable=SC1091
	source /etc/os-release
	if [[ ${ID:-} != cachyos && ${ID_LIKE:-} != *arch* && ${ID:-} != arch ]]; then
		printf 'Uyari: %s resmi hedef degil; Arch uyumlulugu varsayiliyor.\n' "${PRETTY_NAME:-Bu sistem}" >&2
	fi
fi

as_root() {
	if ((EUID == 0)); then
		"$@"
	elif command -v sudo >/dev/null 2>&1; then
		sudo "$@"
	else
		printf 'Root islemi icin sudo bulunamadi: %s\n' "$*" >&2
		return 1
	fi
}

backup_and_install() {
	local source=$1 target=$2 mode=${3:-644}
	local backup="${target}.before-noirlang"
	mkdir -p "$(dirname -- "$target")"
	if [[ -e $target && ! -e $backup ]]; then
		cp -a -- "$target" "$backup"
	fi
	install -m "$mode" -- "$source" "$target"
}

packages=(
	base-devel
	libx11
	libxft
	libxinerama
	fontconfig
	freetype2
	xorg-server
	xorg-xinit
	xorg-xrandr
	xorg-xrdb
	xorg-xset
	xorg-xsetroot
	xorg-xprop
	lightdm
	lightdm-gtk-greeter
	networkmanager
	python-textual
	python-dbus
	wireguard-tools
	bluez
	bluez-utils
	bluez-obex
	dmenu
	rofi
	zenity
	yad
	libnotify
	dunst
	picom
	feh
	maim
	flameshot
	xclip
	slock
	arandr
	wmctrl
	xdotool
	touchegg
	thunar
	thunar-archive-plugin
	tumbler
	gvfs
	gvfs-mtp
	file-roller
	spectacle
	xfce4-power-manager
	brightnessctl
	playerctl
	pamixer
	pavucontrol
	polkit-gnome
	xfce4-settings
	gnome-calculator
	gsimplecal
	gnome-calendar
	dex
	xdg-utils
	xdg-user-dirs
	alacritty
	adw-gtk-theme
	papirus-icon-theme
	breeze
	noto-fonts
	ttf-jetbrains-mono-nerd
)

printf 'CachyOS paketleri kuruluyor...\n'
as_root pacman -Syu --needed --noconfirm "${packages[@]}"

aur_packages=(gazelle-tui bluetuith-bin)
if command -v paru >/dev/null 2>&1; then
	paru -S --needed --noconfirm "${aur_packages[@]}" || true
elif command -v yay >/dev/null 2>&1; then
	yay -S --needed --noconfirm "${aur_packages[@]}" || true
else
	printf 'Uyari: paru/yay bulunamadi; Gazelle ve bluetuith AUR paketlerini elle kurun.\n' >&2
fi

printf 'dwm derleniyor...\n'
make -C "$REPO_ROOT" clean all
as_root make -C "$REPO_ROOT" install

printf 'noirLang yardimci komutlari kuruluyor...\n'
for script in "$REPO_ROOT"/scripts/noir-*; do
	as_root install -Dm755 "$script" "/usr/local/bin/${script##*/}"
done
as_root install -Dm644 "$REPO_ROOT/scripts/lib/noir-common.sh" \
	/usr/local/lib/noir-dwm/noir-common.sh
as_root install -Dm644 "$REPO_ROOT/packaging/noir-dwm.desktop" \
	/usr/share/xsessions/noir-dwm.desktop
as_root install -Dm644 "$REPO_ROOT/packaging/lightdm-noirlang.conf" \
	/etc/lightdm/lightdm.conf.d/20-noirlang.conf
as_root install -Dm644 "$REPO_ROOT/packaging/lightdm-gtk-greeter.conf" \
	/etc/lightdm/lightdm-gtk-greeter.conf.d/20-noirlang.conf

printf 'Koyu tema ve masaustu ayarlari kuruluyor...\n'
backup_and_install "$REPO_ROOT/config/gtk-3.0/settings.ini" \
	"$HOME/.config/gtk-3.0/settings.ini"
backup_and_install "$REPO_ROOT/config/gtk-4.0/settings.ini" \
	"$HOME/.config/gtk-4.0/settings.ini"
backup_and_install "$REPO_ROOT/config/picom/picom.conf" \
	"$HOME/.config/picom/picom.conf"
backup_and_install "$REPO_ROOT/config/dunst/dunstrc" \
	"$HOME/.config/dunst/dunstrc"
backup_and_install "$REPO_ROOT/config/rofi/config.rasi" \
	"$HOME/.config/rofi/config.rasi"
backup_and_install "$REPO_ROOT/config/stalonetray/stalonetrayrc" \
	"$HOME/.config/stalonetray/stalonetrayrc"
backup_and_install "$REPO_ROOT/config/touchegg/touchegg.conf" \
	"$HOME/.config/touchegg/touchegg.conf"

xdg-user-dirs-update 2>/dev/null || true
"$REPO_ROOT/scripts/noir-theme" || true

printf 'Sistem servisleri etkinlestiriliyor...\n'
for old_display_manager in gdm.service sddm.service lxdm.service; do
	if systemctl is-enabled "$old_display_manager" >/dev/null 2>&1; then
		as_root systemctl disable "$old_display_manager"
	fi
done
as_root systemctl enable NetworkManager.service
as_root systemctl enable bluetooth.service
as_root systemctl enable --now touchegg.service >/dev/null 2>&1 || true
as_root systemctl enable --force lightdm.service
as_root systemctl set-default graphical.target

printf '\nKurulum tamamlandi. LightDM ile "noirLang dwm" oturumunu kullanmak icin sistemi yeniden baslatin.\n'
printf 'Mevcut GTK/Rofi/Dunst/Picom dosyalari ilk calistirmada .before-noirlang uzantisiyla yedeklendi.\n'
