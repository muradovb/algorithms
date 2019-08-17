//
//  main.c
//  os_project4
//
//  Created by Bayram Muradov on 25/12/2018.
//  Copyright © 2018 Bayram Muradov. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/shm.h>

//global variables
int cylinders[5000];
int headpos;
int fd_output;
//scheduling algorithms
void start_fcfs(int in_count) {
    int mv_count=0;
    int difference;
    int fcfs_queue [5010];
    fcfs_queue[0]=headpos;
    for(int i=1; i<=in_count; i++) {
        fcfs_queue[i]=cylinders[i];
    }
    
    for(int i=0;i<in_count; i++)
    {
        difference=abs(fcfs_queue[i+1]-fcfs_queue[i]);
        mv_count+=difference;
    }
    //output resls
    printf("%s %d \n", "SSTF:", mv_count);
}

void start_sstf(int in_count) {
    int sstf_dist[5000]; //array keeps distances from headpos
    int sstf_elem[5000]; //array keeps sorted dataset
    //distance from head to each element
    for(int i=0; i<in_count; i++){
        sstf_dist[i] = abs(headpos-cylinders[i]);
    }
    for(int i=0; i<in_count; i++){
        sstf_elem[i] = cylinders[i];
    }
    //sort the dataset elements according to their distance
    //fill the result to local array
    int temp;
    for(int i=0; i<in_count; i++){
        for(int j=i+1; j<in_count;j++){
            if(sstf_dist[i]>sstf_elem[j]){
                temp = sstf_dist[i];
                sstf_dist[i]=sstf_elem[j];
                sstf_dist[j]=temp;
                
                temp=sstf_elem[i];
                sstf_elem[i]=sstf_elem[j];
                sstf_elem[j]=temp;
            }
        }
        
    }
    //calculate movecounts
    int mov_count=0;
    int sstf_head=headpos;
    for(int i=1; i<in_count; i++){
        mov_count = mov_count+abs(sstf_head-sstf_elem[i]);
        sstf_head = sstf_elem[i];
    }
    //output resls
    printf("%s %d \n", "SSTF:", mov_count);
}

void start_scan(int in_count) {
    int queue [5010];
    int head, max, q_size, temp, sum;
    int dloc; //location of disk (head) arr
    q_size=in_count;
    head=headpos;
    
    //fill elements to local queue

    for(int i=0; i<q_size; i++){
        queue[i]=cylinders[i];
    }
    
    queue[q_size] = head; //add RW head into queue
    q_size++;
    
    //sort the array
    for(int i=0; i<q_size;i++){
        for(int j=i; j<q_size; j++){
            if(queue[i]>queue[j]){
                temp = queue[i];
                queue[i] = queue[j];
                queue[j] = temp;
            }
        }
    }
    
    max = queue[q_size-1];
    
    //locate head in the queue
    for(int i=0; i<q_size; i++){
        if(head == queue[i]){
            dloc = i;
            break;
        }
    }
    sum  = head + max;
    printf("\nmovement of total cylinders %d", sum);

}

void start_cscan(int in_count) {
    int LOW=0;
    int HIGH=5000;
    int queue[5010], q_size, head, i,j, seek=0, diff, temp, queue1[5010], queue2[5010], temp1=0, temp2=0;
    float avg;
    int datas[5010];
    q_size=in_count;
    head=headpos;
    
    for(int k=0; k<in_count;k++) {
        datas[k]=cylinders[k];
    }
    
    for(i=0; i<q_size; i++) {
        temp=datas[i];
        if(temp >= head){
            queue1[temp1] = temp;
            temp1++;
        } else {
            queue2[temp2] = temp;
            temp2++;
        }
    }
    //sort both arrays
    for(i=0; i<temp1-1; i++){
        for(j=i+1; j<temp1; j++){
            if(queue1[i] > queue1[j]){
                temp = queue1[i];
                queue1[i] = queue1[j];
                queue1[j] = temp;
            }
        }
    }
    
    for(i=0; i<temp2-1; i++){
        for(j=i+1; j<temp2; j++){
            if(queue2[i]>queue2[j]){
                temp = queue2[i];
                queue2[i] = queue2[j];
                queue2[j] = temp;
            }
        }
    }
    
    //calculate closest edge
    if(abs(head-LOW) >= abs(head-HIGH)){
        
        for(i=1,j=0; j<temp1; i++,j++){
            queue[i] = queue1[j];
        }
        
        queue[i] = HIGH;
        queue[i+1] = 0;
        
        for(i=temp1+3, j=0; j<temp2; i++, j++){
            queue[i] = queue2[j];
        }
        
    } else {
        
        for(i=1,j=temp2-1; j>=0; i++,j--){
            queue[i] = queue2[j];
        }
        
        queue[i] = LOW;
        queue[i+1] = HIGH;
        
        for(i=temp2+3, j=temp1-1; j>=0; i++, j--){
            queue[i] = queue1[j];
        }
    }
    
    queue[0] = head;
    
    for(j=0; j<=q_size+1; j++){
        diff=abs(queue[j+1] - queue[j]);
        seek += diff;
        printf("Disk head moves from %d to %d with seek %d\n",queue[j],queue[j+1],diff);
        
    }
    
    //seek = seek - max; //subtract seek time back to zero
    printf("Total seek time is %d\n", seek);
    avg = seek/(float)q_size;
    printf("Average seek time is %f\n", avg);
    
}

void start_look(int in_count) {
    int LOW=0;
    int HIGH=5000;
    
    int queue[5010], head, q_size, i,j, seek=0, temp, queue1[5010], queue2[5010], temp1=0, temp2=0;
    float avg;
    
    q_size=in_count;
    
    head=headpos;
    int datas[5010];
    for(int i=0; i<in_count; i++) {
        datas[i]=cylinders[i];
    }
    for(i=0; i<q_size; i++){
        temp=datas[i];
        //queue1 - elems greater than head
        if(temp >= head){
            queue1[temp1] = temp;
            temp1++;
        } else {
            queue2[temp2] = temp;
            temp2++;
        }
    }
    
    //sort queue1 - increasing order
    for(i=0; i<temp1-1; i++){
        for(j=i+1; j<temp1; j++){
            if(queue1[i] > queue1[j]){
                temp = queue1[i];
                queue1[i] = queue1[j];
                queue1[j] = temp;
            }
        }
    }
    
    //sort queue2 - decreasing order
    for(i=0; i<temp2-1; i++){
        for(j=i+1; j<temp2; j++){
            if(queue2[i] < queue2[j]){
                temp = queue2[i];
                queue2[i] = queue2[j];
                queue2[j] = temp;
            }
        }
    }
    
    if(abs(head-LOW) >= abs(head-HIGH)){
        
        for(i=1,j=0; j<temp1; i++,j++){
            queue[i] = queue1[j];
        }
        
        for(i=temp1+1, j=0; j<temp2; i++, j++){
            queue[i] = queue2[j];
        }
        
    } else {
        
        for(i=1,j=0; j<temp2; i++,j++){
            queue[i] = queue2[j];
        }
        
        for(i=temp2+1, j=0; j<temp1; i++, j++){
            queue[i] = queue1[j];
        }
        
    }
    
    queue[0] = head;
    
    printf("Total seek time is %d\n", seek);
    avg = seek/(float)q_size;
    printf("Average seek time is %f\n", avg);
}

void start_clook(int in_count) {
    int LOW=0;
    int HIGH=5000;
    int queue[5010], head, q_size, i,j, seek=0, temp, queue1[5010], queue2[5010], temp1=0, temp2=0;
    int datas[5010];
    q_size=in_count;
    
    head=headpos;
    

    for(int i=0; i<in_count; i++) {
        datas[i]=cylinders[i];
    }
    for(i=0; i<q_size; i++){
        temp=datas[i];
        //queue1 - elems greater than head
        if(temp >= head){
            queue1[temp1] = temp;
            temp1++;
        } else {
            queue2[temp2] = temp;
            temp2++;
        }
    }
    
    
    //sort queue1 - increasing order
    for(i=0; i<temp1-1; i++){
        for(j=i+1; j<temp1; j++){
            if(queue1[i] > queue1[j]){
                temp = queue1[i];
                queue1[i] = queue1[j];
                queue1[j] = temp;
            }
        }
    }
    
    //sort queue2
    for(i=0; i<temp2-1; i++){
        for(j=i+1; j<temp2; j++){
            if(queue2[i] > queue2[j]){
                temp = queue2[i];
                queue2[i] = queue2[j];
                queue2[j] = temp;
            }
        }
    }
    
    if(abs(head-LOW) <= abs(head-HIGH)){
        
        for(i=1,j=temp2-1; j>=0; i++,j--){
            queue[i] = queue2[j];
        }
        
        queue[i] = LOW;
        queue[i+1] = HIGH;
        
        for(i=temp2+3,j=temp1-1; j>=0; i++,j--){
            queue[i] = queue1[j];
        }
        
    } else {
        
        for(i=1,j=0; j<temp1; i++,j++){
            queue[i] = queue1[j];
        }
        
        queue[i] = HIGH;
        queue[i+1] = LOW;
        
        for(i=temp1+3,j=0; j<temp2; i++,j++){
            queue[i] = queue2[j];
        }
        
        
    }
    queue[0] = head;
    
    //range = max - min;
    //printf("Range is %d", range);
    //seek =  seek - (max - min);
    
    printf("CLOOK: %d\n", seek);
}

//generates output from specified input set
void generateFromDataset(int inp_count) {
    int in_count=inp_count;
    start_fcfs(in_count);
    start_sstf(in_count);
    start_scan(in_count);
    start_cscan(in_count);
    start_look(in_count);
    start_clook(in_count);
}

//generates output randomly
void generateRandomly() {
    //fill cylinders array randomly
    
}

int createFile(char* filename) {
    int fd_results;
    fd_results=open(filename, O_CREAT | O_RDWR, 0600);
    return fd_results;
}

//main method
int main(int argc, const char * argv[]) {
    if(argc<1) {
        printf("# of args should be @least 1\n");
        return 0;
    }
    char*outname="initialhead";
    strcat(outname, iota(argv[1]));
    fd_output=createFile(outname);
    //terminal args.
    char* FILENAME=NULL;
    
    //assign values
    headpos = atoi(argv[1]);
    if(argc==1) {
        generateRandomly();
    }
    else {
        FILENAME = argv[2];
        //read files and fill global cylinders array
        FILE *fp;
        fp=fopen(FILENAME, "r");
        if(fp==NULL) {
            printf("error while opening file\n");
            exit(1);
        }
        char* temp_buff[100];
        int tracker_inputno=0;
        int temp;
        while(fscanf(fp, "%s", &temp_buff)!= EOF) {
            temp=atoi(temp_buff);
            cylinders[tracker_inputno]=temp;
            tracker_inputno++;
        }
        fclose(fp);
        generateFromDataset(tracker_inputno);
    }
}


