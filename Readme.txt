1.Cài thư viện GTK+
	sudo apt-get install libgtk2.0-dev
2.Biên dịch với dòng lệnh:
	gcc -Wall -g tudienbtl.c -o tudienbtl `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -lm libbt.a