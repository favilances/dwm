#!/usr/bin/env bash

# Shared UI helpers for the small dwm status scripts.

noir_notify() {
	local urgency=${1:-normal}
	local title=${2:-noirLang}
	local body=${3:-}

	if command -v notify-send >/dev/null 2>&1; then
		notify-send -u "$urgency" "$title" "$body"
	fi
}

noir_menu() {
	local prompt=${1:-Sec}

	if command -v dmenu >/dev/null 2>&1; then
		dmenu -i -p "$prompt"
	elif command -v rofi >/dev/null 2>&1; then
		rofi -dmenu -i -no-custom -p "$prompt"
	elif command -v zenity >/dev/null 2>&1; then
		local -a rows=()
		mapfile -t rows
		((${#rows[@]})) || return 1
		zenity --list --title="$prompt" --text="$prompt" \
			--column=Secim --hide-header "${rows[@]}"
	else
		noir_notify critical noirLang "rofi, dmenu veya zenity bulunamadi."
		return 1
	fi
}

noir_password() {
	local prompt=${1:-Parola}

	if command -v rofi >/dev/null 2>&1; then
		printf '' | rofi -dmenu -password -p "$prompt"
	elif command -v zenity >/dev/null 2>&1; then
		zenity --password --title="$prompt"
	else
		noir_notify critical noirLang "Parola sorusu icin rofi veya zenity bulunamadi."
		return 1
	fi
}

noir_confirm() {
	local title=${1:-Onay}
	local question=${2:-Devam edilsin mi?}

	if command -v zenity >/dev/null 2>&1; then
		zenity --question --title="$title" --text="$question" \
			--ok-label=Evet --cancel-label=Hayir
	elif command -v rofi >/dev/null 2>&1; then
		[[ $(printf 'Evet\nHayir\n' | rofi -dmenu -i -no-custom -p "$question") == Evet ]]
	else
		return 1
	fi
}

noir_click_tray_class() {
	local class=$1 win key value tray_pid conf runtime x y
	local X= Y= WIDTH= HEIGHT=
	shift || true
	command -v xdotool >/dev/null 2>&1 || return 1

	click_visible() {
		win=$(xdotool search --all --onlyvisible --class "$class" 2>/dev/null | head -n 1) || return 1
		[[ -n $win ]] || return 1
		while IFS='=' read -r key value; do
			case $key in
				X | Y | WIDTH | HEIGHT) printf -v "$key" '%s' "$value" ;;
			esac
		done < <(xdotool getwindowgeometry --shell "$win" 2>/dev/null)
		[[ $X =~ ^-?[0-9]+$ && $Y =~ ^-?[0-9]+$ && $WIDTH =~ ^[0-9]+$ && $HEIGHT =~ ^[0-9]+$ ]] \
			|| return 1
		xdotool mousemove --sync "$((X + WIDTH / 2))" "$((Y + HEIGHT / 2))" click 1
	}

	click_visible && return 0
	command -v stalonetray >/dev/null 2>&1 || return 1
	while IFS='=' read -r key value; do
		case $key in
			X) x=$value ;;
			Y) y=$value ;;
		esac
	done < <(xdotool getmouselocation --shell 2>/dev/null)
	[[ $x =~ ^[0-9]+$ && $y =~ ^[0-9]+$ ]] || {
		x=0
		y=0
	}
	runtime=${XDG_RUNTIME_DIR:-/tmp}
	conf=$runtime/noir-tray-${class}.${UID}
	{
		printf 'decorations none\n'
		printf 'geometry 1x1+%s+%s\n' "$x" "$y"
		printf 'max_geometry 1x1\n'
		printf 'background "#171a20"\n'
		printf 'kludges force_icons_size\n'
		printf 'icon_size 16\n'
		printf 'slot_size 16\n'
		printf 'sticky true\n'
		printf 'window_strut none\n'
		printf 'window_type utility\n'
		printf 'window_layer top\n'
		printf 'skip_taskbar true\n'
	} > "$conf"
	stalonetray --config "$conf" >/dev/null 2>&1 &
	tray_pid=$!
	( sleep 5; kill "$tray_pid" 2>/dev/null || true; rm -f "$conf" ) &
	if (($#)); then
		"$@" >/dev/null 2>&1 &
	fi
	for _ in 1 2 3 4 5 6 7 8 9 10; do
		sleep 0.15
		click_visible && return 0
	done
	return 1
}

noir_run_checked() {
	local title=$1
	shift
	local output

	if output=$("$@" 2>&1); then
		[[ -z $output ]] || noir_notify normal "$title" "$output"
		return 0
	fi

	noir_notify critical "$title" "${output:-Islem tamamlanamadi.}"
	return 1
}

noir_open_terminal() {
	local terminal

	for terminal in "${TERMINAL:-}" alacritty foot kitty wezterm xterm; do
		[[ -n $terminal ]] || continue
		if command -v "$terminal" >/dev/null 2>&1; then
			"$terminal" -e "$@" &
			return 0
		fi
	done

	noir_notify critical noirLang "Terminal bulunamadi."
	return 1
}

noir_open_tui() {
	local class=$1 title=$2
	shift 2

	if command -v alacritty >/dev/null 2>&1; then
		setsid -f alacritty --class "$class,$class" --title "$title" \
			-o window.dimensions.columns=100 -o window.dimensions.lines=32 -e "$@" >/dev/null 2>&1
	elif command -v kitty >/dev/null 2>&1; then
		setsid -f kitty --class "$class" --title "$title" -e "$@" >/dev/null 2>&1
	elif command -v ghostty >/dev/null 2>&1; then
		setsid -f ghostty --class "$class" --title "$title" -e "$@" >/dev/null 2>&1
	else
		noir_open_terminal "$@"
	fi
}
