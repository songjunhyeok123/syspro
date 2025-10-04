#ifndef STUDENT_H
#define STUDENT_H

#define NAME_MAX 24

typedef struct {
	int id;
	char name[NAME_MAX];
	short score;
}student;

#endif
