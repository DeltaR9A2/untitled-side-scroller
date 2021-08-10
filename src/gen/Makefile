
COMMAND = sed s:TYPE:$(TYPE):g <$< >$@

SRCDIR := ../
OBJDIR := ../../obj/

LISTS := rect item bullet
LIST_SOURCES := $(LISTS:%=$(SRCDIR)/%_list.c)
LIST_HEADERS := $(LISTS:%=$(SRCDIR)/%_list.h)

DICTS := fset anim event target image map
DICT_SOURCES := $(DICTS:%=$(SRCDIR)/%_dict.c)
DICT_HEADERS := $(DICTS:%=$(SRCDIR)/%_dict.h)

ALL_HEADERS := $(LIST_HEADERS) $(DICT_HEADERS)
ALL_SOURCES := $(LIST_SOURCES) $(DICT_SOURCES)

OBJECTS := $(ALL_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o) 

ALL_FILES := $(ALL_HEADERS) $(ALL_SOURCES) $(OBJECTS)

REMOVE  := rm -f

all: $(ALL_FILES)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LISTS): $(LIST_HEADERS) $(LIST_SOURCES)

$(LIST_HEADERS): list_template.h
	$(eval TYPE := $(@F:%_list.h=%))
	$(COMMAND)

$(LIST_SOURCES): list_template.c
	$(eval TYPE := $(@F:%_list.c=%))
	$(COMMAND)

$(DICTS): $(DICT_HEADERS) $(DICT_SOURCES)

$(DICT_HEADERS): dict_template.h
	$(eval TYPE := $(@F:%_dict.h=%))
	$(COMMAND)

$(DICT_SOURCES): dict_template.c
	$(eval TYPE := $(@F:%_dict.c=%))
	$(COMMAND)

clean:
	$(REMOVE) $(ALL_FILES)

