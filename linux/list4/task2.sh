#!/usr/bin/env bash
set -euo pipefail

PLAYER=${PLAYER:-mpg123}
UI=${1:-whiptail}
MUSIC_DIR=${2:-.}

TRACKS=()

collect_tracks() {
  mapfile -d '' -t TRACKS < <(
    find "$MUSIC_DIR" -maxdepth 1 -type f \
      \( -iname '*.mp3' -o -iname '*.ogg' -o -iname '*.flac' -o -iname '*.wav' -o -iname '*.m4a' \) \
      -printf '%f\0' | sort -z
  )

  ((${#TRACKS[@]})) || {
    printf 'No audio files found in directory: %s\n' "$MUSIC_DIR" >&2
    exit 1
  }
}

play_track() {
  command -v "$PLAYER" >/dev/null 2>&1 || {
    printf 'Player not found: %s\n' "$PLAYER" >&2
    exit 1
  }

  "$PLAYER" -- "$MUSIC_DIR/$1"
}

choose_text_menu() {
  local program=$1
  local items=()
  local track

  for track in "${TRACKS[@]}"; do
    items+=("$track" "")
  done

  if [[ $program == "whiptail" ]]; then
    whiptail --title "mp3play / whiptail" \
             --menu "Choose a track" 22 90 14 \
             --output-fd 1 \
             "${items[@]}"
  else
    dialog --clear --stdout \
           --title "mp3play / dialog" \
           --menu "Choose a track" 22 90 14 \
           "${items[@]}"
  fi
}

choose_gui_menu() {
  local program=$1
  local rows=()
  local i
  local idx

  for i in "${!TRACKS[@]}"; do
    rows+=("$i" "${TRACKS[i]}")
  done

  if [[ $program == "zenity" ]]; then
    idx=$(
      zenity --list \
             --title="mp3play / zenity" \
             --text="Choose a track" \
             --width=900 --height=600 \
             --column="ID" --column="Track" \
             --hide-column=1 --print-column=1 \
             "${rows[@]}" \
             2> /dev/null
    ) || return 1
  else
    idx=$(
      yad --list \
          --title="mp3play / yad" \
          --text="Choose a track" \
          --width=900 --height=600 \
          --column="ID:NUM" --column="Track:TEXT" \
          --hide-column=1 --print-column=1 \
          --separator="" \
          --search-column=2 \
          "${rows[@]}" \
          2> /dev/null
    ) || return 1

    idx=${idx%%|*}
  fi

  printf '%s\n' "${TRACKS[$idx]}"
}

choose_track() {
  case "$UI" in
    whiptail|dialog)
      choose_text_menu "$UI"
      ;;
    zenity|yad)
      choose_gui_menu "$UI"
      ;;
    *)
      printf 'Usage: %s {whiptail|dialog|zenity|yad} [directory]\n' "$0" >&2
      exit 2
      ;;
  esac
}

main() {
  local choice

  collect_tracks

  while true; do
    choice=$(choose_track) || break
    play_track "$choice"
  done
}

main "$@"
