#ifndef __RUNNINGAVG__
#define __RUNNINGAVG__


/* Class declaration */
class RunningAvg
{
private:
    int _last_avg = 0;
    int _avg_count = 0;
public:
    Running_avg();
    void add_to_running_avg(int x);
    int get_running_avg();
    int get_count();
};


RunningAvg::Running_avg();

/* Insert new value into running average */
void RunningAvg::add_to_running_avg(int x)
{
    if (_avg_count <= 0)
    {
        this->_last_avg = x;
        this->_avg_count = 1;
    }
    else
    {
        this->_avg_count ++;
        this->_last_avg = (this->_avg_count - 1) / (this->_avg_count * this->_last_avg) + x/this->_avg_count;
    }
}

/* Number of elements currently contributing to running average */
int RunningAvg::get_running_avg()
{
    return this->_last_avg;
}

int RunningAvg::get_count()
{
    return this->_avg_count;
}


#endif