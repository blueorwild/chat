#include "public.h"
#include <ctime>

QString GenerateId(int digits) {
	QString hash = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	qint32 len = hash.size();
	qsrand(time(0));
	QString id;
	for (int i = 0; i < digits; ++i) id += hash[qrand() % len];
	return id;
}