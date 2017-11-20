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

SRCS		=	src/test.c

OBJS		=	$(SRCS:.c=.o)

CFLAGS		+=	-W -Wall -Wextra -fPIC
CFLAGS		+=	-I include/

GCC		=	gcc

RM		=	rm -f


all		:	$(MODULE)

$(MODULE)	:	$(OBJS)
			$(GCC) -shared -o $(MODULE) $(OBJS)

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			$(RM) $(MODULE)

re		:	fclean all

.PHONY		:	all clean fclean re
