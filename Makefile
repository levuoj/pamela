##
## Makefile for  in /home/levuoj/rendu/pamela
##
## Made by Jouvel Anthony
## Login   <anthony.jouvel@epitech.eu>
##
## Started on  Tue Nov 21 18:14:11 2017 Jouvel Anthony
## Last update Tue Nov 21 22:04:38 2017 Jouvel Anthony
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

install		:
			@echo "${MODULE}: installation in progress ..."
			@sleep 1
			@echo "OK"

uninstall	:
			@echo "uninstall ${MODULE}"
			@sleep 1
			@echo "OK"

check		:
			@echo "${MODULE}: checking ..."
			@sleep 1
			@echo "OK"

.PHONY		:	all clean fclean re install uninstall check test
