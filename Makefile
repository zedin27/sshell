sshell: sshell.c
	gcc -Wall -Werror -Wextra -o sshell sshell.c

clean:
	rm sshell
