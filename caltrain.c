#include <pthread.h>
#include "caltrain.h"



void station_init(struct station *station){
	pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->cond_trainArrive, NULL);
    pthread_cond_init(&station->cond_trainCanLeave, NULL);
    station->waiting = 0;
    station->availSeats = 0;
    station->allowed = 0;
}

void station_load_train(struct station *station, int count){

    pthread_mutex_lock(&station->mutex);

    //train have no empty seats or no waiting passengers, then train moves immediately.
    if(station->waiting == 0 || count == 0){
        pthread_mutex_unlock(&station->mutex);
        return;
    }
    else{
        station->availSeats = count;
        pthread_cond_broadcast(&station->cond_trainArrive);
        pthread_cond_wait(&station->cond_trainCanLeave, &station->mutex);
        station->availSeats = 0;

    }

    pthread_mutex_unlock(&station->mutex);

    return;
}

void station_wait_for_train(struct station *station){

	pthread_mutex_lock(&station->mutex);

	station->waiting++;
	// Waiting for train to come.
    while(station->availSeats == 0 )
        pthread_cond_wait(&station->cond_trainArrive, &station->mutex);
    station->availSeats--;
    station->waiting--;
    station->allowed++;

	pthread_mutex_unlock(&station->mutex);

    return;
}

void station_on_board(struct station *station){

    pthread_mutex_lock(&station->mutex);

    station->allowed--;
    // All passengers are on board.
    if(station->allowed == 0 && (station->availSeats == 0 || station->waiting == 0))
        pthread_cond_signal(&station->cond_trainCanLeave);

    pthread_mutex_unlock(&station->mutex);

    return;
}
