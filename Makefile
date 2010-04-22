snd: *.c
	gcc `pkg-config --libs --cflags sndfile` snd_analytics.c -o snd
