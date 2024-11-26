#!/bin/bash

set +x

function read_msg () {
	position=1
	slice=$(echo "$1" | cut -d' ' -f$((position + 1))-)
	if [[ "$slice" == "liste" ]] then
		array=($(ls))
		read_command array
	elif [[ "$slice" == "qui suis-je" ]] then
		echo "Tu es $2"
	elif [[ "$slice" == "au revoir" ]] then
		killall chat-bot.sh
	elif [[ "$(echo "$1" | awk '{print $2}')" == "li" ]] then
		array=()
		input="$(echo "$1" | awk '{print $3}')"
		while IFS= read -r line
		do
			array+=("$line")
		done < "$input"
		read_command array
		
	else
		input="liste-bot.txt"
		while IFS= read -r line
		do
			if [[ "$(echo "$line" | awk '{print $1}')" == "$(echo "$1" | awk '{print $2}')" ]] then
				second_word=$(echo "$line" | cut -d' ' -f$((position + 1))-)
				echo "$second_word"
				return 0
			fi
		done < "$input"
		echo -e '\xF0\x9F\xA4\x96 ?'
	fi
}

function read_command() {
    local -n arr=$1  
    for elem in "${arr[@]}"; do
        echo "$elem" >&"${CHAT[1]}"
		sleep .01
    done
	echo "" >&"${CHAT[1]}"
}


Interlocutor="$1"
coproc CHAT { ./chat bot "$Interlocutor" --bot; }
read <&"${CHAT[0]}" 
while true; do
	read msg <&"${CHAT[0]}" 
	read_msg "$msg" "$Interlocutor" >&"${CHAT[1]}" 2>&"${CHAT[1]}"
done
