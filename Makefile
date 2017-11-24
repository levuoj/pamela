
##
## Makefile for  in /home/levuoj/rendu/pamela
##
## Made by Jouvel Anthony
## Login   <anthony.jouvel@epitech.eu>
##
## Started on  Tue Nov 21 18:14:11 2017 Jouvel Anthony
## Last update Thu Nov 23 09:16:18 2017 pamela
##

GCC		=	gcc

RM		=	rm -f

MODULE		=	pam_test.so

_SRCS		=	account/account.c

_SRCS		+=	auth/pam_sm_authenticate.c \
			auth/pam_sm_setcred.c

_SRCS		+=	converse/converse.c

_SRCS		+=	password/password.c

_SRCS		+=	session/session.c

_SRCS		+=	utils/utils.c

SRCS		=	$(addprefix src/, $(_SRCS))

OBJS		=	$(SRCS:.c=.o)

CFLAGS		+=	-W -Wall -Wextra
CFLAGS		+=	-fPIC
CFLAGS		+=	-g
CFLAGS		+=	-I include/

LIB_PAM		=	-lpam
LIB_PAM_MISC	=	-lpam_misc


all		:	$(MODULE)

$(MODULE)	:	$(OBJS)
			$(GCC) -shared -o $(MODULE) $(OBJS) $(LIB_PAM) $(LIB_PAM_MISC)

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			$(RM) $(MODULE)

re		:	fclean all

install		:	fclean uninstall all
			@echo "${MODULE}: installation in progress ..."
			sudo cp ${MODULE} /lib/i386-linux-gnu/security/
#			sudo echo " " > conf pam
			@echo "OK"

uninstall	:
			@echo "uninstall ${MODULE}"
			sudo rm /lib/i386-linux-gnu/security/${MODULE}
#			sudo head -n -1 /etc/pam.d/conf > ./conf.temp ; sudo mv ./conf.temp etc/pam.d/conf
			@echo "OK"

check		:
			@echo "${MODULE}: checking ..."
			@sleep 1
			@echo "OK"

.PHONY		:	all clean fclean re install uninstall check
