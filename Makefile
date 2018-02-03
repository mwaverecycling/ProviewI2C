testioproj_top : testioproj

include $(pwr_exe)/pwrp_rules.mk

testioproj_modules = $(pwrp_obj)/ra_io_m_randomfilereaderio.o $(pwrp_obj)/ra_io_m_customethernetio.o $(pwrp_obj)/rt_io_user.o

# Main rule
testioproj : $(testioproj_modules)
	@ echo "** Modules Built **"

# Modules
$(pwrp_obj)/ra_io_m_randomfilereaderio.o : $(pwrp_src)/ra_io_m_randomfilereaderio.c $(pwrp_inc)/pwr_cvolmark2classes.h
$(pwrp_obj)/ra_io_m_customethernetio.o : $(pwrp_src)/ra_io_m_customethernetio.c $(pwrp_inc)/pwr_cvolmark2classes.h
$(pwrp_obj)/rt_io_user.o : $(pwrp_src)/rt_io_user.c

#	gcc -g -c rt_io_user.c -o $(pwrp_obj)/rt_io_user.o -I$(pwrp_inc) -I$(pwr_inc) -DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86 -DOS_POSIX
#	gcc -g -c ra_io_m_randomfilereaderio.c -o $(pwrp_obj)/ra_io_m_randomfilereaderio.o -I$(pwrp_inc) -I$(pwr_inc) -DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86 -DOS_POSIX
