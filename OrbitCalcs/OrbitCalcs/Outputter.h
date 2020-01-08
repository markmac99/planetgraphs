#pragma once

double MyRound(double input, int places, int updown);
void addHeader(FILE* f, char* pname, char* typ);
void addFooter(FILE* f, char* unt, char* typ, double minv, double maxv);
