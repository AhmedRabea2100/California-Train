#include <pthread.h>


struct station {
    int waiting, availSeats, allowed;
	pthread_mutex_t mutex;
    pthread_cond_t cond_trainArrive, cond_trainCanLeave;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
