
CC = gcc
BASICOPTS = -g
CFLAGS = -Werror -Wall -g -std=c11 -D_DEFAULT_SOURCE


TARGETDIR_hashtable.so=hashtable


all: $(TARGETDIR_hashtable.so)/hashtable.so copy end

CFLAGS_hashtable.so = 
OBJS_hashtable.so =  \
	$(TARGETDIR_hashtable.so)/hashtable.o



SHAREDLIB_FLAGS_hashtable.so = -shared 
$(TARGETDIR_hashtable.so)/hashtable.so: $(TARGETDIR_hashtable.so) $(OBJS_hashtable.so) 
	$(CC) $(CFLAGS) $(CFLAGS_hashtable.so) -o $@ $(OBJS_hashtable.so) $(SHAREDLIB_FLAGS_hashtable.so)


$(TARGETDIR_hashtable.so)/hashtable.o: $(TARGETDIR_hashtable.so) hashtable.c
	$(CC) $(CFLAGS) -c $(CFLAGS_hashtable.so) $(CPPFLAGS_hashtable.so) -o $@ hashtable.c

clean:
	rm -f \
		$(TARGETDIR_hashtable.so)/hashtable.so \
		$(TARGETDIR_hashtable.so)/hashtable.o 
	rm -f -r $(TARGETDIR_hashtable.so)

copy:
	cp hashtable.h $(TARGETDIR_hashtable.so)/
	
end:
	rm -f $(TARGETDIR_hashtable.so)/hashtable.o	

$(TARGETDIR_hashtable.so):
	mkdir -p $(TARGETDIR_hashtable.so)

.KEEP_STATE:
