#include<bits/stdc++.h>
#define SIZE 100
#define INSTANCE 3
#define TIMEQUANTUM 4
using namespace std;

int sysResourceA;
int sysResourceB;
int sysResourceC;
int sysMemorySize;
int sysFramesize;
int availableFrame;

class Process
{
private:
    int processID;
    string processName;
    string processStatus;
    int processSize;
    int hasIOoperation; // 1 means yes, 0 means no
    int burst;
    int arrivalTime;
    int priority;
    static int processCounter;
    int allocatedResource[INSTANCE];
    int maxResource[INSTANCE];
    int needResource[INSTANCE];

public:
    Process() {}
    void setProcess(string processName, int processSize, int hasIOoperation, int burst, int arrivalTime, int priority,int allocatedResource[], int maxResource[])
    {
        this->processID = ++(this->processCounter);
        this->processName = processName;
        this->processStatus = "new";
        this->processSize = processSize;
        this->hasIOoperation = hasIOoperation;
        this->burst = burst;
        this->arrivalTime = arrivalTime;
        this->priority = priority;
        for(int i=0; i<INSTANCE; i++)
        {
            this->allocatedResource[i]=allocatedResource[i];
            this->maxResource[i] = maxResource[i];
        }
    }
    void setProcessID(int processID)
    {
        this->processID = processID;
    }
    void setProcessName(string processName)
    {
        this->processName = processName;
    }
    void setProcessStatus(string processStatus)
    {
        this->processStatus = processStatus;
    }
    void setProcessSize(int processSize)
    {
        this->processSize = processSize;
    }
    void setHasIOoperation(int hasIOoperation)
    {
        this->hasIOoperation = hasIOoperation;
    }
    void setAllocatedResource(int allocatedResource[])
    {
        for(int i=0; i<INSTANCE; i++)
        {
            this ->allocatedResource[i] = allocatedResource[i];
        }
    }
    void setMaxResource(int maxResource[])
    {
        for(int i=0; i<INSTANCE; i++)
        {
            this->maxResource[i]=maxResource[i];
        }
    }
    void calculateNeedResource()
    {
        for(int i=0; i<INSTANCE; i++)
        {
            this->needResource[i]= maxResource[i]-allocatedResource[i];
        }

    }
    void setBurst(int burst)
    {
        this->burst = burst;
    }
    void setArrivalTime(int arrivalTime)
    {
        this->arrivalTime = arrivalTime;
    }
    void setPriority(int priority)
    {
        this->priority = priority;
    }
    int getProcessID()
    {
        return this->processID;
    }
    string getProcessName()
    {
        return this->processName;
    }
    string getProcessStatus()
    {
        return this->processStatus;
    }
    int getProcessSize()
    {
        return this->processSize;
    }
    int getHasIOoperation()
    {
        return this->hasIOoperation;
    }
    int* getAllocatedResource()
    {
        return this->allocatedResource;
    }
    int* getMaxResource()
    {
        return this->maxResource;
    }
    int* getNeedResource()
    {
        return this->needResource;
    }
    int getBurst()
    {
        return this->burst;
    }
    int getArrivalTime()
    {
        return this->arrivalTime;
    }
    int getPriority()
    {
        return this->priority;
    }
};

int Process::processCounter = 0;

vector<Process> JOB_QUEUE;
vector<Process> READY_QUEUE;
vector<Process> DEVICE_QUEUE;
vector<Process> TERMINATED_QUEUE;

int GANTT_CHART[1000];

int job_queue_front, job_queue_rear = 0;
int ready_queue_front, ready_queue_rear = 0;
int device_queue_front, device_queue_rear = 0;
int terminated_queue_front, terminated_queue_rear = 0;

void initGanttChart()
{
    for(int i=0; i<1000; i++)
        GANTT_CHART[i] = -1;
}

void createProcess()
{
    int pSize, pOp, pBurst, pArrivalTime, pPriority,pAllocation[INSTANCE],pMax[INSTANCE],k=65;
    char c;
    Process ob;
    string pName;
    cout << "Enter Process Name: ";
    cin >> pName;
    cout << "Enter Process Size: ";
    cin >> pSize;
    cout << "Has IO operation? (1 for yes, 0 for no): ";
    cin >> pOp;

    if(pOp==0)
    {
        for(int i=0; i<INSTANCE; i++)
        {
            pAllocation[i]=0;
        }
        for(int i=0; i<INSTANCE; i++)
        {

            pMax[i]=0;
        }
    }
    else
    {
        cout << "Enter the Resource has been allocated:" << endl;
        for(int i=0; i<INSTANCE; i++)
        {
            c=k+i;
            cout << c << ": ";
            cin >> pAllocation[i];
        }
        cout << "Enter the Resource Maximum:" << endl;
        for(int i=0; i<INSTANCE; i++)
        {
            c=k+i;
            cout << c << ": " ;
            cin >> pMax[i];
        }

    }

    cout << "Enter Burst: ";
    cin >> pBurst;
    cout << "Enter Arrival Time: ";
    cin >> pArrivalTime;
    cout << "Enter Priority: ";
    cin >> pPriority;
	ob.setProcess(pName, pSize, pOp, pBurst, pArrivalTime, pPriority,pAllocation,pMax);
	JOB_QUEUE.push_back(ob);
	job_queue_rear++;
}

void showJobQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Job Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival"
         << setw(10) << "Priority" << setw(10) << "Size" << setw(10) << "hasIO?" << setw(10) << "Allocated Resource\t" << setw(10) << "Max Resource"<<endl;
    for(int i = 0; i < job_queue_rear; i++)
    {
        cout << setw(10) << JOB_QUEUE[i].getProcessID() << setw(10) << JOB_QUEUE[i].getProcessName()
             << setw(10) << JOB_QUEUE[i].getBurst() << setw(10) << JOB_QUEUE[i].getArrivalTime()
             << setw(10) << JOB_QUEUE[i].getPriority()
             << setw(10) << JOB_QUEUE[i].getProcessSize()
             << setw(10) << JOB_QUEUE[i].getHasIOoperation()
             << setw(10);
        int* Allocated = JOB_QUEUE[i].getAllocatedResource();
        int* Max = JOB_QUEUE[i].getMaxResource();
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Allocated;
            Allocated++;
        }
        cout << setw(10);
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Max;
            Max++;
        }
        cout << endl;

    }

}

void showReadyQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Ready Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival" << setw(10) << "Priority"
         << setw(10) << "Size" << setw(10) << "hasIO?" << setw(10) << "Allocated Resource\t" << setw(10) << "Max Resource"<<endl;
    for(int i = 0; i < ready_queue_rear; i++)
    {
        cout << setw(10) << READY_QUEUE[i].getProcessID() << setw(10) << READY_QUEUE[i].getProcessName()
             << setw(10) << READY_QUEUE[i].getBurst() << setw(10) << READY_QUEUE[i].getArrivalTime()
             << setw(10) << READY_QUEUE[i].getPriority()
             << setw(10) << READY_QUEUE[i].getProcessSize()
             << setw(10) << READY_QUEUE[i].getHasIOoperation()
             << setw(10);
        int* Allocated = READY_QUEUE[i].getAllocatedResource();
        int* Max = READY_QUEUE[i].getMaxResource();
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Allocated;
            Allocated++;
        }
        cout << setw(10);
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Max;
            Max++;
        }
        cout << endl;

    }


}

void showDeviceQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Device Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival" << setw(10) << "Priority"
         << setw(10) << "Size" << setw(10) << "hasIO?" << setw(10) << "Allocated Resource\t" << setw(10) << "Max Resource"<<endl;
    for(int i = 0; i < device_queue_rear; i++)
    {
        cout << setw(10) << DEVICE_QUEUE[i].getProcessID() << setw(10) << DEVICE_QUEUE[i].getProcessName()
             << setw(10) << DEVICE_QUEUE[i].getBurst() << setw(10) << DEVICE_QUEUE[i].getArrivalTime()
             << setw(10) << DEVICE_QUEUE[i].getPriority()
             << setw(10) << DEVICE_QUEUE[i].getProcessSize()
             << setw(10) << DEVICE_QUEUE[i].getHasIOoperation()
             << setw(10);
        int* Allocated = DEVICE_QUEUE[i].getAllocatedResource();
        int* Max = DEVICE_QUEUE[i].getMaxResource();
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Allocated;
            Allocated++;
        }
        cout << setw(10);
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Max;
            Max++;
        }
        cout << endl;
    }

}

void showTerminatedProcess()
{

    cout.setf(ios::left, ios::adjustfield);
    cout << "Recent Terminated Process" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival" << setw(10) << "Priority"
         << setw(10) << "Size" << setw(10) << "hasIO?" << setw(10) << "Allocated Resource\t" << setw(10) << "Max Resource"<<endl;
    for(int i = 0; i < terminated_queue_rear ; i++)
    {
        cout << setw(10) << TERMINATED_QUEUE[i].getProcessID() << setw(10) << TERMINATED_QUEUE[i].getProcessName()
             << setw(10) << TERMINATED_QUEUE[i].getBurst() << setw(10) << TERMINATED_QUEUE[i].getArrivalTime()
             << setw(10) << TERMINATED_QUEUE[i].getPriority()
             << setw(10) << TERMINATED_QUEUE[i].getProcessSize()
             << setw(10) << TERMINATED_QUEUE[i].getHasIOoperation()
             << setw(10);
        int* Allocated = TERMINATED_QUEUE[i].getAllocatedResource();
        int* Max = TERMINATED_QUEUE[i].getMaxResource();
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Allocated;
            Allocated++;
        }
        cout << setw(10);
        for(int j=0; j<INSTANCE; j++)
        {
            cout << "  " << *Max;
            Max++;
        }
        cout << endl;

    }

}

void bankers_algorithm()
{
    cout << "Enter into bankers algorithm" << endl;
    showDeviceQueue();
    int TotalA=0,TotalB=0,TotalC=0,*Allocation, *Need,complete=0;
    int Need_array[INSTANCE];
    bool Finish[device_queue_rear];
    for(int i=0; i<device_queue_rear; i++)
    {
        Finish[i] = false;
        Allocation = DEVICE_QUEUE[i].getAllocatedResource();
        TotalA = TotalA+*Allocation;
        Allocation++;
        TotalB = TotalB + *Allocation;
        Allocation++;
        TotalC = TotalC + *Allocation;
    }
    int work[]= {(sysResourceA-TotalA),(sysResourceB-TotalB), (sysResourceC- TotalC)};
    ///checking*****************
    cout <<    "Initially Available :  ";
    for(int x=0; x<INSTANCE; x++)
    {
        cout << work[x] << "   ";
    }
    cout << endl;

    ///----------------------
    int work_temp[] = {(sysResourceA-TotalA),(sysResourceB-TotalB), (sysResourceC- TotalC)};
    int i=0;
    while(i<device_queue_rear)
    {
        Need = DEVICE_QUEUE[i].getNeedResource();

        for(int j=0; j<INSTANCE; j++)
        {
            Need_array[j]= *Need;
            Need++;
        }

        if(Finish[i]==false && Need_array[0]<=work[0] && Need_array[1]<=work[1] && Need_array[2]<=work[2])
        {
            Allocation = DEVICE_QUEUE[i].getAllocatedResource();
            for(int j=0; j<INSTANCE; j++)
            {
                work[j]=work[j]+*Allocation;
                Allocation++;
            }
            Finish[i]=true;
            complete++;
        }

        if(complete==device_queue_rear)
        {
            cout << "System is safe." << endl;
            break;
        }
        if(i==(device_queue_rear-1))
        {
            if(work_temp[0]==work[0] && work_temp[1]==work[1] && work_temp[2]==work[2])
            {
                cout << "System is Unsafe!" << endl;
                break;
            }
            else
            {
                work_temp[0]=work[0];
                work_temp[1]=work[1];
                work_temp[2]=work[2];
                i=-1;
            }
        }

        i++;
    }
}

void sequence_job_queue(vector <Process> temp)
{
	Process ob;
    int allot[INSTANCE], maxm[INSTANCE];
    int *a, *m;
    int i;
    for(i=0;i<temp.size();i++)
    {
        ob.setProcessID(temp[i].getProcessID());
        ob.setProcessName(temp[i].getProcessName());
        ob.setProcessSize(temp[i].getProcessSize());
        ob.setProcessStatus(temp[i].getProcessStatus());
        ob.setHasIOoperation(temp[i].getHasIOoperation());
        ob.setArrivalTime(temp[i].getArrivalTime());
        ob.setBurst(temp[i].getBurst());
        ob.setPriority(temp[i].getPriority());
        for(int j=0;j<INSTANCE;j++)
        {
            a= temp[i].getAllocatedResource() ;
            allot[j]= *a;
            a++;
        }
        ob.setAllocatedResource(allot);
        for(int j=0;j<INSTANCE;j++)
        {
            m= temp[i].getMaxResource() ;
            maxm[j]= *m;
            m++;
        }
        ob.setMaxResource(maxm);
        JOB_QUEUE.push_back(ob);
    }// End of for

    job_queue_rear = i;
    temp.clear();
}

void sequence_device_queue(vector <Process> temp){

	Process ob;
    int allot[INSTANCE], maxm[INSTANCE];
    int *a, *m;
    int i;
    for(i=0;i<temp.size();i++)
    {
        ob.setProcessID(temp[i].getProcessID());
        ob.setProcessName(temp[i].getProcessName());
        ob.setProcessSize(temp[i].getProcessSize());
        ob.setProcessStatus(temp[i].getProcessStatus());
        ob.setHasIOoperation(temp[i].getHasIOoperation());
        ob.setArrivalTime(temp[i].getArrivalTime());
        ob.setBurst(temp[i].getBurst());
        ob.setPriority(temp[i].getPriority());
        for(int j=0;j<INSTANCE;j++)
        {
            a= temp[i].getAllocatedResource() ;
            allot[j]= *a;
            a++;
        }
        ob.setAllocatedResource(allot);
        for(int j=0;j<INSTANCE;j++)
        {
            m= temp[i].getMaxResource() ;
            maxm[j]= *m;
            m++;
        }
        ob.setMaxResource(maxm);
        DEVICE_QUEUE.push_back(ob);
    }// End of for

    device_queue_rear = i;
    temp.clear();
}
void readyProcess()
{
    cout << "Transferring Processes from Job Queue to Ready/Device Queue." << endl;
    int j = 0, k = 0,*Allocate, *Max;
    int Allocate_array[INSTANCE], Max_array[INSTANCE];
    vector <Process> temp;
    Process ob;
    for(int i = 0; i<job_queue_rear; i++)
    {
        Process P = JOB_QUEUE[i];
        if(P.getHasIOoperation() == 1)
        {
            ob.setProcessID(P.getProcessID());
            ob.setProcessName(P.getProcessName());
            ob.setProcessSize(P.getProcessSize());
            ob.setHasIOoperation(P.getHasIOoperation());
            ob.setProcessStatus("waiting");
            ob.setBurst(P.getBurst());
            ob.setArrivalTime(P.getArrivalTime());
            ob.setPriority(P.getPriority());
            Allocate = P.getAllocatedResource();
            Max = P.getMaxResource();
            for(int n=0; n<INSTANCE; n++)
            {
                Allocate_array[n] = *Allocate;
                Max_array[n] = *Max;
                Allocate++;
                Max++;
            }
            ob.setAllocatedResource(Allocate_array);
            ob.setMaxResource(Max_array);
            ob.calculateNeedResource();
            DEVICE_QUEUE.push_back(ob);
            j++;
        }
        else if(ceil(P.getProcessSize()/sysFramesize)<=availableFrame)
        {
            ob.setProcessID(P.getProcessID());
            ob.setProcessName(P.getProcessName());
            ob.setProcessSize(P.getProcessSize());
            ob.setHasIOoperation(P.getHasIOoperation());
            ob.setProcessStatus("ready");
            ob.setBurst(P.getBurst());
            ob.setArrivalTime(P.getArrivalTime());
            ob.setPriority(P.getPriority());
            Allocate = P.getAllocatedResource();
            Max = P.getMaxResource();
            for(int n=0; n<INSTANCE; n++)
            {
                Allocate_array[n] = *Allocate;
                Max_array[n] = *Max;
                Allocate++;
                Max++;
            }
            ob.setAllocatedResource(Allocate_array);
            ob.setMaxResource(Max_array);
            ob.calculateNeedResource();
            READY_QUEUE.push_back(ob);
            k++;
            float temp = sysFramesize;
            availableFrame=availableFrame-ceil(P.getProcessSize()/temp);
        }
        else
        {
            temp.push_back(P);
        }
    }
    ready_queue_rear = k;
    device_queue_rear = j;
    job_queue_rear = 0;
    cout << "Transfer Completed." << endl;
    ///*************checking
    int s = temp.size();
    for(int i=0;i<s;i++)
    {
        cout << "temp [" << i << "]: " << temp[i].getProcessName() << endl;
    }
    ///-----------------------
    JOB_QUEUE.clear();
    sequence_job_queue(temp);
    if(device_queue_rear > 0)
        bankers_algorithm();
}

void readyIOProcess()
{
    cout << "Transferring Processes from Device Queue to Ready Queue." << endl;
    int j = 0, k = ready_queue_rear,*Allocate, *Max;
    int Allocate_array[INSTANCE], Max_array[INSTANCE];
    vector <Process> temp;
    Process ob;
    for(int i = 0; i<device_queue_rear; i++)
    {
        Process P = DEVICE_QUEUE[i];

        if(ceil(P.getProcessSize()/sysFramesize)<=availableFrame)
        {
            ob.setProcessID(P.getProcessID());
            ob.setProcessName(P.getProcessName());
            ob.setProcessSize(P.getProcessSize());
            ob.setHasIOoperation(P.getHasIOoperation());
            ob.setProcessStatus("ready");
            ob.setBurst(P.getBurst());
            ob.setArrivalTime(P.getArrivalTime());
            ob.setPriority(P.getPriority());
            Allocate = P.getAllocatedResource();
            Max = P.getMaxResource();
            for(int n=0; n<INSTANCE; n++)
            {
                Allocate_array[n] = *Allocate;
                Max_array[n] = *Max;
                Allocate++;
                Max++;
            }
            ob.setAllocatedResource(Allocate_array);
            ob.setMaxResource(Max_array);
            ob.calculateNeedResource();
            READY_QUEUE.push_back(ob);
            k++;
            float temp = sysFramesize;
            availableFrame=availableFrame-ceil(P.getProcessSize()/temp);
        }
        else
        {
            temp.push_back(P);
        }
    }
    ready_queue_rear = k;
    device_queue_rear= 0;
    cout << "Transfer Completed." << endl;
    ///*************checking
    int s = temp.size();
    for(int i=0;i<s;i++)
    {
        cout << "temp [" << i << "]: " << temp[i].getProcessName() << endl;
    }
    ///-----------------------
    DEVICE_QUEUE.clear();
    sequence_device_queue(temp);
}

void terminatedProcess()
{
    int j = 0, k = 0,*Allocate, *Max;
    int Allocate_array[INSTANCE], Max_array[INSTANCE];
    vector <Process> temp;
    Process ob;
    for(int i = 0; i<ready_queue_rear; i++)
    {
        Process P = READY_QUEUE[i];
        ob.setProcessID(P.getProcessID());
        ob.setProcessName(P.getProcessName());
        ob.setProcessSize(P.getProcessSize());
        ob.setHasIOoperation(P.getHasIOoperation());
        ob.setProcessStatus("Terminated");
        ob.setBurst(P.getBurst());
        ob.setArrivalTime(P.getArrivalTime());
        ob.setPriority(P.getPriority());
        Allocate = P.getAllocatedResource();
        Max = P.getMaxResource();
        for(int n=0; n<INSTANCE; n++)
        {
            Allocate_array[n] = *Allocate;
            Max_array[n] = *Max;
            Allocate++;
            Max++;
        }
        ob.setAllocatedResource(Allocate_array);
        ob.setMaxResource(Max_array);
        ob.calculateNeedResource();
        TERMINATED_QUEUE.push_back(ob);
        k++;
    }
    terminated_queue_rear = k;
}

int compare( Process p1, Process p2){
    return p1.getArrivalTime()<p2.getArrivalTime();
}
void displayGanttChart(int clock)
{
    cout.setf(ios::left, ios::adjustfield);
    for(int i=0; i<clock; i++)
    {
        if(i%10==0 && i/10>=1)
            cout << endl;
        cout << setw(3) << GANTT_CHART[i];
    }
    cout << endl;
}

double average(int *list)
{
    int sum = 0;
    for(int i = 0; i<ready_queue_rear; i++)
    {
        sum += list[i];
    }
    return sum*1.0/ready_queue_rear;
}

void Free_memory(){
    int sum=0;
    for(int i=0;i<ready_queue_rear;i++){
        if(READY_QUEUE[i].getProcessStatus()== "Terminated"){
            float temp = sysFramesize;
            sum = sum+ceil(READY_QUEUE[i].getProcessSize()/temp);
        }
    }
    availableFrame = availableFrame+sum;
    ready_queue_rear = 0;
    cout << "Total Frame free: " << sum << endl;
}

void fcfs()
{
    sort( READY_QUEUE.begin(), READY_QUEUE.end(), compare );
    showReadyQueue();
    initGanttChart();
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        if(READY_QUEUE[i].getArrivalTime()<=clock && READY_QUEUE[i].getProcessStatus()!="Terminated")
        {
            for(int j = clock; j < clock + remaining_time[i]; j++)
            {
                GANTT_CHART[j] = READY_QUEUE[i].getProcessID();
            }

            if(seen[i]==false)
            {
                response_time[i] = clock - READY_QUEUE[i].getArrivalTime();
                seen[i]=true;
            }
            clock = clock + READY_QUEUE[i].getBurst();
            READY_QUEUE[i].setProcessStatus("Terminated");
            waiting_time[i] = clock - READY_QUEUE[i].getArrivalTime() - READY_QUEUE[i].getBurst();
            turnaround_time[i] = clock - READY_QUEUE[i].getArrivalTime();
            i++;
            complete++;
        }
        else
        {
            clock++;
        }
    }
    cout << "FCFS Scheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
    terminatedProcess();
    Free_memory(); /// Free those Process's memory which are Terminated
    READY_QUEUE.clear();
}

void sjf()
{
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        int minm = INT_MAX;
        int shortest = -1;
        for(int i=0; i<ready_queue_rear; i++)
        {
            if(READY_QUEUE[i].getBurst() < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getBurst();
                shortest = i;
            }
        }
        // no suitable process found, incrementing clock time
        if(shortest==-1)
        {
            clock++;
            continue;
        }
        
        if(seen[shortest]==false)
        {
            response_time[shortest] = clock - READY_QUEUE[shortest].getArrivalTime();
            seen[shortest]=true;
        }
        clock = clock + READY_QUEUE[shortest].getBurst();
        READY_QUEUE[shortest].setProcessStatus("Terminated");
        waiting_time[shortest] = clock -
                                 READY_QUEUE[shortest].getArrivalTime() -
                                 READY_QUEUE[shortest].getBurst();
        turnaround_time[shortest] = clock -
                                    READY_QUEUE[shortest].getArrivalTime();
        complete++;
    }
    cout << "SJF Scheduling" << endl;
    //displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
    terminatedProcess();
    Free_memory(); /// Free those Process's memory which are Terminated
    READY_QUEUE.clear();
}

void srtf()
{
    sort(READY_QUEUE.begin(), READY_QUEUE.end(), compare);
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        // find next process to execute
        int minm = INT_MAX;
        int highest = -1;
        //bool found = false;
        for(int i=0; i<ready_queue_rear; i++)
        {
            if(remaining_time[i] < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = remaining_time[i];
                highest = i;
            }
        }

        if(highest==-1)
        {
            clock++;
            continue;
        }

        
        if(seen[highest]==false)
        {
            response_time[highest] = clock - READY_QUEUE[highest].getArrivalTime();
            seen[highest]=true;
        }

        if(remaining_time[highest]>0)
        {
            clock++;
            remaining_time[highest]--;
            continue;
        }

        //clock = clock + READY_QUEUE[highest].getBurst();
        READY_QUEUE[highest].setProcessStatus("Terminated");
        waiting_time[highest] = clock -
                                READY_QUEUE[highest].getArrivalTime() -
                                READY_QUEUE[highest].getBurst();
        turnaround_time[highest] = clock -
                                   READY_QUEUE[highest].getArrivalTime();
        complete++;
    }
    cout << "Shortest Remaining Time First" << endl;
    //displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
    terminatedProcess();
	Free_memory(); /// Free those Process's memory which are Terminated
    READY_QUEUE.clear();
}

void priorityScheduling()
{
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        // find next process to execute
        int minm = INT_MAX;
        int highest = -1;
        //bool found = false;
        for(int i=0; i<ready_queue_rear; i++)
        {
            if(READY_QUEUE[i].getPriority() < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getBurst();
                highest = i;
            }
        }
        // no suitable process found, incrementing clock time
        if(highest==-1)
        {
            clock++;
            continue;
        }
        
        if(seen[highest]==false)
        {
            response_time[highest] = clock - READY_QUEUE[highest].getArrivalTime();
            seen[highest]=true;
        }
        clock = clock + READY_QUEUE[highest].getBurst();
        READY_QUEUE[highest].setProcessStatus("Terminated");
        waiting_time[highest] = clock -
                                READY_QUEUE[highest].getArrivalTime() -
                                READY_QUEUE[highest].getBurst();
        turnaround_time[highest] = clock -
                                   READY_QUEUE[highest].getArrivalTime();
        complete++;
    }
    cout << "Priority Scheduling" << endl;
    //displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
    terminatedProcess();
	Free_memory(); /// Free those Process's memory which are Terminated
	READY_QUEUE.clear();
}

void preemptivePriorityScheduling()
{
    sort(READY_QUEUE.begin(), READY_QUEUE.end(), compare);

    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        // find next process to execute
        int minm = INT_MAX;
        int highest = -1;
        //bool found = false;
        for(int i=0; i<ready_queue_rear; i++)
        {
            if(READY_QUEUE[i].getPriority() < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getPriority();
                highest = i;
            }
        }

        if(highest==-1)
        {
            clock++;
            continue;
        }

        
        if(seen[highest]==false)
        {
            response_time[highest] = clock - READY_QUEUE[highest].getArrivalTime();
            seen[highest]=true;
        }

        if(remaining_time[highest]>0)
        {
            clock++;
            remaining_time[highest]--;
            continue;
        }

        //clock = clock + READY_QUEUE[highest].getBurst();
        READY_QUEUE[highest].setProcessStatus("Terminated");
        waiting_time[highest] = clock -
                                READY_QUEUE[highest].getArrivalTime() -
                                READY_QUEUE[highest].getBurst();
        turnaround_time[highest] = clock -
                                   READY_QUEUE[highest].getArrivalTime();
        complete++;
    }
    cout << "Preemptive Priority Scheduling Scheduling" << endl;
    //displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
    terminatedProcess();
	Free_memory(); /// Free those Process's memory which are Terminated
	READY_QUEUE.clear();
}

void roundRobin()
{
    sort(READY_QUEUE.begin(), READY_QUEUE.end(),compare);
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {

        //int highest = -1;
        cout << "Inside while loop: " << "\nClock: " << clock << endl;

        for(int i=0; i<ready_queue_rear; i++)
        {
            if(READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                cout << "Process Name: " << READY_QUEUE[i].getProcessName() << endl;
                cout << "Process Remaining time: " << remaining_time[i] << endl;
                cout << "Clock : " << clock << endl;

                if(remaining_time[i]>TIMEQUANTUM)
                {
                    cout << "Updating the clock: " << endl;
                    cout << "current clock: " << clock << endl;
                    cout << "TIMEQUANTUM: " << TIMEQUANTUM << endl;
                    cout << "Remaining time: "<< remaining_time[i] << endl;
                    clock+= TIMEQUANTUM;
                    cout << "Clock updated: " << clock << endl;
                    remaining_time[i]-= TIMEQUANTUM;
                    cout << "Remaining time updated: " << remaining_time[i] << endl;
                }
                else
                {
                    cout << "clock : " << clock << endl;
                    cout << "Remaining time: " << remaining_time[i] << endl;
                    clock = clock + remaining_time[i];
                    READY_QUEUE[i].setProcessStatus("Terminated");
                    waiting_time[i] = clock -
                                READY_QUEUE[i].getArrivalTime() -
                                READY_QUEUE[i].getBurst();
                    turnaround_time[i] = clock -
                                   READY_QUEUE[i].getArrivalTime();
                    complete++;
                    cout << "waiting time [" << i << "] :" << waiting_time[i] << endl;
                    cout << "Complete++" << complete << endl;

                }
            }
            else if(READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                cout << "Inside the clock++" << endl;
                clock++;
            }
        }

        

    }//End of while
    cout << "Preemptive Priority Scheduling Scheduling" << endl;
    //displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    //cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
    terminatedProcess();
	Free_memory(); /// Free those Process's memory which are Terminated
	READY_QUEUE.clear();
}

void executeProcess()
{
    int choice;
    cout << "CPU Scheduling Algorithm Selection" << endl;
    cout << "-------------------" << endl;
    cout << "(1) First-Come-First-Served (FCFS)" << endl;
    cout << "(2) Shortest-Job-First (SJF)" << endl;
    cout << "(3) Preemptive SJF (shortest-remaining-time-first" << endl;
    cout << "(4) Priority-based Scheduling" << endl;
    cout << "(5) Preemptive Priority-based Scheduling" << endl;
    cout << "(6) Round-Robin Scheduling" << endl;
    cout << "Enter Choice: ";
    cin >> choice;
    switch(choice)
    {
    case 1:
        fcfs();
        break;
    case 2:
        sjf();
        break;
    case 3:
        srtf();
        break;
    case 4:
        priorityScheduling();
        break;
    case 5:
        preemptivePriorityScheduling();
        break;
    case 6:
        roundRobin();
        break;
    default:
        cout << "Unrecognized Option. " << endl;
    }
}



void reexecuteProcess()
{
    int j = 0, k = 0,*Allocate, *Max;
    int Allocate_array[INSTANCE], Max_array[INSTANCE];
    vector <Process> temp;
    Process ob;
    for(int i = 0; i<terminated_queue_rear; i++)
    {
        Process P = TERMINATED_QUEUE[i];
        ob.setProcessID(P.getProcessID());
        ob.setProcessName(P.getProcessName());
        ob.setProcessSize(P.getProcessSize());
        ob.setHasIOoperation(P.getHasIOoperation());
        ob.setProcessStatus("ready");
        ob.setBurst(P.getBurst());
        ob.setArrivalTime(P.getArrivalTime());
        ob.setPriority(P.getPriority());
        Allocate = P.getAllocatedResource();
        Max = P.getMaxResource();
        for(int n=0; n<INSTANCE; n++)
        {
            Allocate_array[n] = *Allocate;
            Max_array[n] = *Max;
            Allocate++;
            Max++;
        }
        ob.setAllocatedResource(Allocate_array);
        ob.setMaxResource(Max_array);
        ob.calculateNeedResource();
        READY_QUEUE.push_back(ob);
        float temp = sysFramesize;
        availableFrame=availableFrame-ceil(P.getProcessSize()/temp);
        k++;
    }
    ready_queue_rear = k;
    executeProcess();
}

void sysConfig()
{
    cout << "Enter number of instances of A: ";
    cin >> sysResourceA;
    cout << "Enter number of instances of B: ";
    cin >> sysResourceB;
    cout << "Enter number of instances of C: ";
    cin >> sysResourceC;
    cout << "Enter memory size: ";
    cin >> sysMemorySize;
    cout << "Enter system Frame size: ";
    cin >> sysFramesize;
    availableFrame = (sysMemorySize/sysFramesize);
}

int main()
{
    string command;
    cout << "Configure the system:- " << endl;
    sysConfig();
    do
    {
        cout << "> ";
        cin >> command;
        if(command=="create-process")
        {
            createProcess();
        }
        else if(command=="ready-process")
        {
            readyProcess();
        }
        else if(command=="ready-IO-process")
        {
            readyIOProcess();
        }
        else if(command=="show-job-queue")
        {
            showJobQueue();
        }
        else if(command=="show-ready-queue")
        {
            showReadyQueue();
        }

        else if(command=="show-device-queue")
        {
            showDeviceQueue();
        }
        else if(command=="show-terminated-process")
        {
            showTerminatedProcess();
        }
        else if(command=="execute-process")
        {
            executeProcess();
        }
        else if(command=="reexecute-process")
        {
            reexecuteProcess();
        }
        else if(command=="exit")
        {
            cout << "Thank You." << endl;
            exit(0);
        }
        else
        {
            cout << "Unrecognized Command. Try again." << endl;
        }
    }
    while(true);
}
