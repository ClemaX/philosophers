NAME = philosophers

# Compiler
CC = clang
LD = clang

# Paths
SRCDIR=srcs
INCDIR=includes

OBJDIR=objs
BINDIR=.

# Flags
CFLAGS = -Wall -Wextra -I$(INCDIR) -g3
DFLAGS = -MT $@ -MMD -MP -MF $(OBJDIR)/$*.d
LDFLAGS = -lpthread

SRCS = $(addprefix $(SRCDIR)/,\
	observer.c\
	philo_actions.c\
	philo.c\
	philosophers.c\
	table_utils.c\
	table.c\
	utils_int.c\
	utils_str.c\
	utils_time.c\
)

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

COMPILE.c = $(CC) $(DFLAGS) $(CFLAGS) -c
COMPILE.o = $(LD) $(LDFLAGS)

all: $(BINDIR)/$(NAME)

# Directories
$(OBJDIR) $(BINDIR):
	@echo "MK $@"
	mkdir -p "$@"

# Objects
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)/%.d | $(OBJDIR)
	@mkdir -p '$(@D)'
	@echo "CC $<"
	$(COMPILE.c) $< -o $@

# Dependencies
$(DEPS): $(OBJDIR)/%.d:
include $(wildcard $(DEPS))

# Binaries
$(BINDIR)/$(NAME): $(OBJS) | $(BINDIR)
	@echo "LD $@"
	$(COMPILE.o) $^ -o $@ $(LDLIBS)

clean:
	@echo "RM $(OBJDIR)"
	rm -rf "$(OBJDIR)"

fclean: clean
	@echo "RM $(BINDIR)/$(NAME)"
	rm -f "$(BINDIR)/$(NAME)"
	@rmdir "$(BINDIR)" 2>/dev/null && echo "RM $(BINDIR)" || :

re: fclean all

.PHONY: clean fclean re

# Assign a value to VERBOSE to enable verbose output
$(VERBOSE).SILENT:
