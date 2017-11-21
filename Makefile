##
## Makefile
##
##
## Made by Pashervz
##
##
## 20/11/2017
##

MODULE		=	pam_test.so

SRCS		=	src/main.c

OBJS		=	$(SRCS:.c=.o)

CFLAGS		+=	-W -Wall -Wextra -fPIC
CFLAGS		+=	-I include/

GCC		=	gcc

RM		=	rm -f

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

.PHONY		:	all clean fclean re
