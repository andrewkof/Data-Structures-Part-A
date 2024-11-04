/*
 * ============================================
 * file: main.c
 * @Author Kofidis Andreas (kofidis@csd.uoc.gr)
 * @Version 23/10/2023
 *
 *
 * @brief Main function
 *        Winter Semester 2023-2024
 * @see   Compile using supplied Makefile by running: make
 * ============================================
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "streaming_service.h"

/* Maximum input line size */
#define MAX_LINE 1024

/* 
 * Uncomment the following line to
 * enable debugging prints
 * or comment to disable it
 */
/* #define DEBUG */
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else
#define DPRINT(...)
#endif /* DEBUG */

struct user *user_list = NULL;
struct movie movie_categories[6] = {};
struct new_movie *new_movies = NULL;

void init_structures(void){

    struct user *new_node = malloc(sizeof *new_node);
    new_node->uid = -1;
    new_node->suggestedHead = NULL;
    new_node->suggestedTail = NULL;
    new_node->watchHistory = NULL;
    new_node->next = NULL;
    user_list = new_node;
    int i; 

    for(i = 0; i < 6; i++){
        movie_categories[i].info.mid = -1;
    }
    
    
	/*
	 * TODO: Initialize your
	 * global structures here,
	 * i.e., the user list (and sentinel
	 * node), new releases list, category
	 * table
	 */
}


void destroy_structures(void)
{
    struct user *u = user_list, *tmp;
    while(u){
        tmp = u;
        u = u->next;
        free(tmp);
    }

    /* Check memory leaks */
/* valgrind ./cs240StreamingService test_U5M10 --leak-check=full */

	/*
	 * TODO: For a bonus
	 * empty all lists and stacks
	 * and free all memory associated
	 * with list/stack nodes here
	 */
}



int main(int argc, char *argv[])
{
	FILE *event_file;
	char line_buffer[MAX_LINE];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	event_file = fopen(argv[1], "r");
	if (!event_file) {
		perror("fopen error for event file open");
		exit(EXIT_FAILURE);
	}


	init_structures();
    printf("Check\n");
	while (fgets(line_buffer, MAX_LINE, event_file)) {
		char *trimmed_line;
		char event;
		int uid;
		unsigned mid, year;
		movieCategory_t category1, category2;
		/*
		 * First trim any whitespace
		 * leading the line.
		 */
		trimmed_line = line_buffer;
		while (trimmed_line && isspace(*trimmed_line))
			trimmed_line++;
		if (!trimmed_line)
			continue;
		/* 
		 * Find the event,
		 * or comment starting with #
		 */
		if (sscanf(trimmed_line, "%c", &event) != 1) {
			fprintf(stderr, "Could not parse event type out of input line:\n\t%s",
					trimmed_line);
			fclose(event_file);
			exit(EXIT_FAILURE);
		}

		switch (event) {
			/* Comment, ignore this line */
			case '#':
				break;
			case 'R':
				if (sscanf(trimmed_line, "R %d", &uid) != 1) {
					fprintf(stderr, "Event R parsing error\n");
					break;
				}
				register_user(uid);
				break;
			case 'U':
				if (sscanf(trimmed_line, "U %d", &uid) != 1) {
                    fprintf(stderr, "Event U parsing error\n");
					break;
				}
				unregister_user(uid);
				break;
			case 'A':
				if (sscanf(trimmed_line, "A %u %d %u", &mid, &category1,
							&year) != 3) {
					fprintf(stderr, "Event A parsing error\n");
					break;
				}
				add_new_movie(mid, category1, year);
				break;
			case 'D':
				distribute_new_movies();
				break;
			case 'W':
				if (sscanf(trimmed_line, "W %d %u", &uid, &mid) != 2) {
					fprintf(stderr, "Event W parsing error\n");
					break;
				}
				watch_movie(uid, mid);
				break;
			case 'S':
				if (sscanf(trimmed_line, "S %d", &uid) != 1) {
					fprintf(stderr, "Event S parsing error\n");
					break;
				}
				suggest_movies(uid);
				break;
			case 'F':
				if (sscanf(trimmed_line, "F %d %d %d %u", &uid, &category1,
							&category2, &year) != 4) {
					fprintf(stderr, "Event F parsing error\n");
					break;
				}
				filtered_movie_search(uid, category1, category2, year);
				break;
			case 'T':
				if (sscanf(trimmed_line, "T %u", &mid) != 1) {
					fprintf(stderr, "Event T parsing error\n");
					break;
				}
				take_off_movie(mid);
				break;
			case 'M':
				print_movies();
				break;
			case 'P':
				print_users();
				break;
			default:
				fprintf(stderr, "WARNING: Unrecognized event %c. Continuing...\n",
						event);
				break;
		}
	}
	fclose(event_file);
	destroy_structures();
	return 0;
}

void unregister_user(int uid){
    struct user *p = user_list;
    struct user *p_1 = p->next;
    while(p){
        if(p->uid == uid){
            struct suggested_movie *s = p->suggestedHead, *tmp1;
            while(s){
                tmp1 = s;
                s = s->next;
                free(tmp1);
            }

            struct movie *w = p->watchHistory, *tmp2;
            while(w){
                tmp2 = w;
                w = w->next;
                free(tmp2);
            }
            if(p == user_list){
                user_list = p->next;
            }else{
                // p_1->next = p->next;
                // free(p);
                // return;
                struct user *prev = user_list;
                while (prev->next != p) {
                    prev = prev->next;
                }
                prev->next = p->next;
            }
            free(p);
        }
        p = p->next;
    }
    printf("U %d\n", uid);
    struct user *k = user_list;
    printf("\tUsers = %d", k->uid);
    k = k->next;
    while(k){
        printf(", %d", k->uid);
        k = k->next;
    }
    printf("\nDONE\n");
    return;
}


void take_off_movie(unsigned mid){
    printf("T %d\n", mid);
    struct user *p = user_list;
    while (p){
        struct suggested_movie *s = p->suggestedHead, *tmp;
        while(s){
            if (s->info.mid == mid){
                tmp = s;
                if(s->prev){
                    s->prev->next = s->next;

                } else {
                    p->suggestedHead = s->next; 
                }

                if(s->next){
                    s->next->prev = s->prev;
                } else {
                    p->suggestedTail = s->prev; 
                }
                printf("\t%d removed from %d suggested list.\n", mid, p->uid);
                free(tmp);
            }
            s=s->next;
        }
        p = p->next;
    }

    int i, cat;
    for(i = 0; i < 6; i++){
        struct movie *h = &movie_categories[i];
        struct movie *n = h->next;
        if(!h){
            continue;
        }
        if(!n){
            if(h->info.mid == mid){
                movie_categories[i].info.mid = -1; 
                break;
            }
        }else{
            while(n){
                if(h->info.mid == mid){
                    movie_categories[i] = *n;
                    goto x;
                }else if(n->info.mid == mid){
                    struct movie *t = h;
                    h->next = n->next;
                    cat = i;
                    goto x;
                }
                h = h->next;
                n = n->next;
            }
        }

    }
x:
    printf("\t%u removed from %d category list\n", mid, cat);
    struct movie *h = &movie_categories[i];
    int c = 1;
    printf("\tCategory list = %u_%d", h->info.mid, c);
    h = h->next;
    while(h){
        c++;
        printf(", %u_%d", h->info.mid, c);
        h = h->next;
    }
    printf("\nDONE\n");
}

void print_users(void){
    struct user *p = user_list;
    printf("\nP\n");
    printf("\tUsers:\n");

    while(p){             /* -1 einai o komvos frouros (sto telos tis listas)*/
        if (p->uid==-1){
            printf("\nDONE\n");
            return;
        }
        printf("\t\t%d:\n", p->uid);
        if (p->suggestedHead){
            int c1 = 1;
            struct suggested_movie *s = p->suggestedHead;
            printf("\t\t\tSuggested: %d_%d", s->info.mid, 1);
            s = s->next;
            while(s){
                c1++;
                printf(", %d_%d", s->info.mid, c1);
                s = s->next;
            }
        }
        else{
            printf("\t\t\tSuggested:\n");
        }
        if(p->watchHistory){
            int c2 = 1;
            struct movie *m = p->watchHistory;
            printf("\n\t\t\tWatch History: %d_%d", m->info.mid, c2);
            m=m->next;
            while(m){
                c2++;
                printf(", %d_%d", m->info.mid, c2);
                m = m->next;
            }
            printf("\n");
        }
        else{
            printf("\t\t\tWatch History:\n");
        }
        p = p->next;
    }

}
int filtered_movie_search(int uid, movieCategory_t category1,
		movieCategory_t category2, unsigned year){
    
    struct movie *p1, *p2;
    struct suggested_movie *new_list = NULL, *tail;
    if(movie_categories[category1].info.mid != -1 && movie_categories[category2].info.mid != -1){
        p1 = &movie_categories[category1];
        p2 = &movie_categories[category2];
        while(p1 && p2){
            if(p1->info.mid <= p2->info.mid){
                if(p1->info.year >= year){
                    struct suggested_movie *new_node = malloc(sizeof *new_node);
                    new_node->info.mid = p1->info.mid;
                    new_node->info.year = p1->info.year;

                    if(!new_list){
                        new_list = new_node;
                        new_node->next = NULL;
                        new_node->prev = NULL;
                    }else{
                        new_node->next = NULL;
                        new_node->prev = tail;
                        tail->next = new_node;
                    }
                    tail = new_node;
                }
                p1 = p1->next;
            }else if(p2->info.mid <= p1->info.mid){
                if(p2->info.year >= year){
                    struct suggested_movie *new_node = malloc(sizeof *new_node);
                    new_node->info.mid = p2->info.mid;
                    new_node->info.year = p2->info.year;

                    if(!new_list){
                        new_list = new_node;
                        new_node->next = NULL;
                        new_node->prev = NULL;
                    }else{
                        new_node->next = NULL;
                        new_node->prev = tail;
                        tail->next = new_node;
                    }
                    tail = new_node;
                }
                p2 = p2->next;
            }
        }
        if(p1){
            while(p1){
                struct suggested_movie *new_node = malloc(sizeof *new_node);
                new_node->info.mid = p1->info.mid;
                new_node->info.year = p1->info.year;
                new_node->next = NULL;
                new_node->prev = tail;
                tail->next = new_node;
                tail = new_node;
                p1 = p1->next;
            }
        }else if(p2){
            while(p2){
                struct suggested_movie *new_node = malloc(sizeof *new_node);
                new_node->info.mid = p2->info.mid;
                new_node->info.year = p2->info.year;
                new_node->next = NULL;
                new_node->prev = tail;
                tail->next = new_node;
                tail = new_node;
                p2 = p2->next;
            }
        }
    }else if(movie_categories[category1].info.mid == -1 
        && movie_categories[category2].info.mid == -1){
        printf("\nBoth categories are empty!\n");
        return -1;
    }

    if(movie_categories[category1].info.mid == -1){
        p2 = &movie_categories[category2];
        while(p2){
            struct suggested_movie *new_node = malloc(sizeof *new_node);
            new_node->info.mid = p2->info.mid;
            new_node->info.year = p2->info.year;
            new_node->next = NULL;
            new_node->prev = tail;
            tail = new_node;
            p2 = p2->next;
        }
    }

    if(movie_categories[category2].info.mid == -1){
        p1 = &movie_categories[category1];
        while(p1){
            struct suggested_movie *new_node = malloc(sizeof *new_node);
            new_node->info.mid = p1->info.mid;
            new_node->info.year = p1->info.year;
            new_node->next = NULL;
            new_node->prev = tail;
            tail = new_node;
            p1 = p1->next;
        }
    }

    /* Find user and connect the new double linked list to their suggested Movies */
    struct user *our_user, *f = user_list;
    while(f){
        if(uid == f->uid){
            our_user = f;
            break;
        }
        f = f->next;
    }

    if(our_user->suggestedHead == NULL){
        our_user->suggestedHead = new_list;
        our_user->suggestedTail = tail;
    }else{
        our_user->suggestedTail->next = new_list;
        new_list->prev = our_user->suggestedTail;
        our_user->suggestedTail = tail;
    }

    printf("F %d %d %d %u\n", uid, category1, category2, year);
    struct suggested_movie *h = our_user->suggestedHead;
    int c = 1;
    printf("\tUser %d Suggested Movies = %u_%d", uid, h->info.mid, c);
    h = h->next;
    while(h){
        c++;
        printf(", %u_%d", h->info.mid, c);
        h = h->next;
    }
    printf("\nDONE\n");


    return 0;
}

int suggest_movies(int uid){

    struct user *p = user_list;
    struct user *our_user;
    while(p){
        if(p->uid == uid){
            our_user = p; 
            break;
        }
        p = p->next;
    }
    if(!our_user){
        printf("User %d not found!\n", uid);
        return -1;
    }

    unsigned m;
    int n = 1;
    p = user_list;
    struct suggested_movie *h;
    struct suggested_movie *t;
    while(p){
        if(p->uid != uid){
            if(p->watchHistory){
                m = p->watchHistory->info.mid;
                struct suggested_movie *new_node = malloc(sizeof *new_node);
                new_node->info.mid = m;
                new_node->prev = NULL;
                new_node->next = NULL;
                if(n % 2 != 0){
                    if(n == 1){
                        our_user->suggestedHead = new_node;
                        h = new_node;
                    }else{
                        h->next = new_node;
                        new_node->prev = h;
                        h = new_node;
                    }
                }else{
                    if(n == 2){
                        our_user->suggestedTail = new_node;
                        t = new_node;
                    }else{
                        new_node->next = t;
                        t->prev = new_node;
                        t = new_node;
                    }
                }
                n++;
            }
        }
        p = p->next;
    }
        h->next = t;
        t->prev = h;

    
    printf("\nS %d\n", uid);
    printf("\tUser %d Suggested Movies = %d_1", uid, our_user->suggestedHead->info.mid);
    h = our_user->suggestedHead->next;
    int c = 2;
    while(h){
        printf(", %d_%d", h->info.mid, c);
        c++;
        h = h->next;
    }
    printf("\nDONE\n");
    return 0;
}

void print_movie_category(struct movie *movie_categories, int i){
    struct movie *tmp;
    int c;
    if(movie_categories[i].info.mid != -1){
        c = 1;
        printf("%d_%d,%d", movie_categories[i].info.mid, i+1, c);
        tmp = movie_categories[i].next;
        while(tmp){
            c++;
            printf(" ,%d_%d,%d", tmp->info.mid, i+1, c);
            tmp = tmp->next;
        }
    }
    return;
}

void print_movies(void){
    printf("Categorized Movies:\n");
    printf("\tHorror: ");
    print_movie_category(movie_categories, 0);

    printf("\n");
    printf("\tSci-Fi:");
    print_movie_category(movie_categories, 1);

    printf("\n");
    printf("\tDrama:");
    print_movie_category(movie_categories, 2);
    
    printf("\n");
    printf("\tRomance:");
    print_movie_category(movie_categories, 3);

    printf("\n");
    printf("\tDocumentary:");
    print_movie_category(movie_categories, 4);
    
    printf("\n");
    printf("\tComedy:");
    print_movie_category(movie_categories, 5);

    printf("\nDONE\n");
}

int watch_movie(int uid, unsigned mid){
    int i;
    struct movie *p;
    bool movie_exists = false, user_exists = false;
    for(i = 0; i < 6; i++){
        if(movie_categories[i].info.mid != -1){
            p = &movie_categories[i];
            while(p){
                if(p->info.mid == mid){
                    movie_exists = true;
                    break;
                }
                p = p->next;
            }
            if(movie_exists) break;
        }
        
    }
    if(!movie_exists){
        printf("\nMovie %u does not exist!\n", mid);
        return -1;
    }

    struct user *h = user_list; 
    while(h){
        if(h->uid == uid){
            user_exists = true;
            break;
        }
        h = h->next;
    }
    if(!user_exists){
        printf("User does not exist!\n");
        return -1;
    }
    
    struct movie *new_node = malloc(sizeof *new_node);
    new_node->info.mid = mid;
    new_node->next = h->watchHistory;
    h->watchHistory = new_node;

    printf("\nW %d %u\n", uid, mid);
    printf("User %d Watch History = %d_1 ", uid, new_node->info.mid);
    struct movie *t = h->watchHistory;
    t = t->next;
    int c = 2;
    while(t){
        printf(",%d_%d ", t->info.mid, c);
        t = t->next;
        c++;
    }
    printf("\nDONE\n");
    return 0;
}
void distribute_new_movies(void){
    struct new_movie *p = new_movies;
    struct new_movie *old_head;
    struct movie *help[6] = {};
    struct movie *tmp;
    int c;
    while(p){
        if(movie_categories[p->category].info.mid == -1){
            printf("Category0: %d\n", p->category);
            movie_categories[p->category].info.mid = p->info.mid;
            movie_categories[p->category].info.year = p->info.year;
            movie_categories[p->category].next = NULL;

            help[p->category] = &movie_categories[p->category];
        }else{
            printf("Category: %d\n", p->category);
            struct movie *new_node = malloc(sizeof *new_node);
            new_node->info.mid = p->info.mid;
            new_node->info.year = p->info.year;
            new_node->next = NULL;

            help[p->category]->next = new_node;
            help[p->category] = new_node;
            help[p->category]->next = NULL;

        }
        old_head = p;
        /* an mpei edw h free(old_head) tote tha svistei o p diladi to head ara tha xaseis ti lista*/
        p = p->next;
        free(old_head);
    }
    printf("Categorized Movies:\n");
    printf("\tHorror: ");
    print_movie_category(movie_categories, 0);

    printf("\n");
    printf("\tSci-Fi:");
    print_movie_category(movie_categories, 1);

    printf("\n");
    printf("\tDrama:");
    print_movie_category(movie_categories, 2);
    
    printf("\n");
    printf("\tRomance:");
    print_movie_category(movie_categories, 3);

    printf("\n");
    printf("\tDocumentary:");
    print_movie_category(movie_categories, 4);
    
    printf("\n");
    printf("\tComedy:");
    print_movie_category(movie_categories, 5);

    printf("\nDONE\n");
    
}


int add_new_movie(unsigned mid, movieCategory_t category, unsigned year){

    struct new_movie *p = new_movies;
    while(p){
        if(mid == p->info.mid){
            printf("Duplicate mid %d\n", mid);
            return -1;
        }
        p = p->next;
    }
    bool flag = false;
    if(!new_movies){
        struct new_movie *new_node = malloc(sizeof *new_node);
        new_node->info.mid = mid;
        new_node->info.year = year;
        new_node->category = category;
        new_node->next = NULL;
        new_movies = new_node;
    }else{
        struct new_movie *new_node = malloc(sizeof *new_node);
        new_node->info.mid = mid;
        new_node->info.year = year;
        new_node->category = category;

        p = new_movies;
        struct new_movie *help;
        struct new_movie *help2;
        
        if(new_node->info.mid < p->info.mid){
            new_node->next = new_movies;
            new_movies = new_node;
        }else{
            while(p){
                if(new_node->info.mid > p->info.mid){
                    help = p;
                    help2 = help->next;
                }
                p = p->next;
            }
            help->next = new_node;
            if(help2){
                new_node->next = help2;
            }else{
                new_node->next = NULL;
            }
        }
    }
    printf("A %d %d %d\n", mid, category, year);
    p = new_movies;

    printf("\tNew movies = %d %d %d", p->info.mid, p->category, p->info.year);
    p = p->next;
    while(p){
        printf(",%d %d %d", p->info.mid, p->category, p->info.year);
        p = p->next;
    }
    printf("\nDONE\n");
    return 0;
}

int register_user(int uid){
    struct user *p = user_list; 
    while(p != NULL){
        if(uid == p->uid){
            printf("R %d\n", uid);
            printf("Duplicate uid: %d\n", uid);
            return -1;
        }
        p = p->next;
    }

    struct user *new_node = malloc(sizeof *new_node);
    new_node->uid = uid;
    new_node->suggestedHead = NULL;
    new_node->suggestedTail = NULL;
    new_node->watchHistory = NULL;
    new_node->next = user_list;
    user_list = new_node;

    printf("R %d\n", uid);
    p = user_list;
    printf("\tUsers = %d", p->uid);
    p = p->next;
    while(p != NULL){
        printf(",%d", p->uid);
        p = p->next;
    }
    printf("\nDONE\n");
    return 0;
}
