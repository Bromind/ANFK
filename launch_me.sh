#!/bin/sh

height=""
width=""
for i in `stty size` ; do 
	height=`echo $width / 1.5 | bc`;
	width=`echo $i / 1.5 | bc`;
done

build ()
{
	workingDirectory=`pwd`
	filepath=`dialog --stdout --title "Select output directory" --fselect "$workingDirectory" $height $width`
	make -C ./kernel 2>&1 | dialog --title "Building the kernel" --programbox $height $width;
	cp kernel/kernel.img kernel/kernel.elf kernel/kernel.map kernel/kernel.list "$filepath"
	main;
}

clean()
{
	make -C ./kernel clean 2>&1 | dialog --title "Cleaning kernel directories" --programbox $height $width;
	rm kernel.*;
	main;
}

update()
{
	(git pull || (echo "This is not a repo, will clone the repo instead." ; git clone https://github.com/Bromind/ANFK.git) )2>&1 | dialog --title "Updating sources" --programbox $height $width;
	main;
}

credit()
{
	dialog --title "Credits" --textbox credit $height $width;
	main;
}

readme()
{
	dialog --title "README.md" --textbox README.md $height $width;
	main;
}

quit()
{
	clear;
}

parseChoice()
{
	case $1 in
		"build") build;;
		"clean") clean;;
		"update") update;;
		"credit") credit;;
		"readme") readme;;
		"quit") quit ;;
	esac
}

main()
{
	dialog --clear --title "K-FetOS/ANFK project builder"\
		--menu "$VERSION" $height $width 10\
		"build" "Build the kernel" \
		"clean" "Clean directories" \
		"update" "Update the sources from github" \
		"credit" "List developers"\
		"readme" "Print readme"\
		"quit" "Quit" 2> $tempfile

	retval=$?

	choice=`cat $tempfile`

	case $retval in
		0) parseChoice "$choice" ;;
		1) clear;;
		255) clear;;
	esac
}

alert()
{
	dialog --title "Architecture alert" --yesno "You don't seem to be on an ARM arch. This project is made for ARM only. Nevertheless, if you want to continue, please consider modifying the Makefiles with your toolchain compiler & assembler.\n\nDo you want to continue anyway ?" $height $width
	if [ $? -eq 0 ]
	then 
		main
	else 
		quit
	fi
}

alert_noDialog()
{
	echo "It seems that you are not equipped with an ARM platform. If you have a toolchain, you should modify the Makefiles with your stuff."
	echo "1. Continue"
	echo "2. Quit"

	read ANSWER;
	case $ANSWER in 
		'1') noDialog;;
		'2') quit;;
		*) quit;;
	esac

}

noDialog()
{
	echo "Entering the Roleplay style builder"
	echo "\n\n"
	echo "[The old peasant] : \t\"Hey you, it seems you don't have 'dialog' installed."
	echo "\t\t\tThe builder heavily rely on that stuff, even if it is not required for compiling/using the kernel."
	echo "\t\t\tYou can have info there : <https://en.wikipedia.org/wiki/Dialog_%28software%29>"
	echo ""
	echo "\t\t\tSo, since you don't want to install decent stuff, I, the old peasant, have to do the building"
	echo "\t\t\tWhat do you want to do ?\""
	echo ""
	echo "[Gamemaster] : \t\"What do you answer to the old peasant ?\""
	echo ""
	echo "1. \"I apologize, and I'd like to build the kernel\""
	echo "2. \"Sorry for that terrible miss, I want to clean directories\""
	echo "3. \"Nonetheless I don't have good stuff, and I have outdated sources, could you please update them ?\""
	echo "4. \"I'd like to pay tribute to the guys who work on that stuff. Please tell me their name? \""
	echo "5. \"Maybe the developers leave another message I should read, can you show me their last words ?\""
	echo "6. \"You're right, I just quit for a second, the time for me to get right stuff \""

	read ANSWER;
	case $ANSWER in
		'1') echo "Let us try to build this stuff" ; make -C ./kernel;;
		'2') echo "Let me clean all the dirty stuff" ; make -C ./kernel clean;; 
		'3') echo "Let me try to get you new stuff" ; git pull;;
		'4') echo "Here are their names" ; cat credit;;
		'5') echo "Here are their last indications : " ; cat README.md;;
		'6') echo "You chosed wisely, come to me when you'll have installed dialog." ; exit;;
		*) echo "I don't understand your answer, I quit" ; exit;;
	esac
}

clear;

tempfile=`mktemp 2> /dev/null` || tempfile=/tmp/ANFK_build.tmp
trap "rm -f $tempfile ; rm -rf ./tmp" 0 1 2 5 15
ARCH=`arch | head -c 3`

REMOTE=`git ls-remote | head -n 1 | cut -f 1`
CURRENT=`git log -1 --format="%H"`

VERSION=`git log -1 --format="Last commit : %h, %ar. "`
if [ "$REMOTE" != "$CURRENT" ]
then 
	UPDATE="Your version is not the same as upstream";
else
	UPDATE="Version up-to-date";
fi
VERSION="$VERSION\n$UPDATE"

if [ $ARCH -ne "arm" ]
then
	type dialog > /dev/null 2>&1 || alert_noDialog;
	alert
else 
	type dialog > /dev/null 2>&1 || noDialog;
	main
fi
