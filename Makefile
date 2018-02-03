include $(pwr_exe)/pwrp_rules.mk



CLASS_HEADER = $(pwrp_inc)/pwr_cvoli2cclasses.h

SRC_DIR = ./src
USER_SRC = $(SRC_DIR)/rt_io_user.c

SOURCES = $(wildcard $(SRC_DIR)/ra_io_m_*.c)
SOURCES += $(USER_SRC)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(pwrp_obj)/%.o, $(SOURCES))



# Main rule
testioproj : $(OBJECTS)
	@echo "** Modules Built **"


# Modules
$(pwrp_obj)/%.o : $(SRC_DIR)/%.c $(CLASS_HEADER)

#	gcc -g -c rt_io_user.c -o $(pwrp_obj)/rt_io_user.o -I$(pwrp_inc) -I$(pwr_inc) -DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86 -DOS_POSIX
#	gcc -g -c ra_io_m_randomfilereaderio.c -o $(pwrp_obj)/ra_io_m_randomfilereaderio.o -I$(pwrp_inc) -I$(pwr_inc) -DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86 -DOS_POSIX
