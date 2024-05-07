/**
 * Project SearchEngine
 */


#ifndef _TIMERMANAGER_H
#define _TIMERMANAGER_H

class TimerManager {
public: 
    
void start();
    
void stop();
    
/**
 * @param TimerTask*
 */
void attach(void TimerTask*);
    
/**
 * @param TimerTask*
 */
void detach(void TimerTask*);
private: 
    int _timerfd;
    vector<TimerTask*> _wheelList;
};

#endif //_TIMERMANAGER_H