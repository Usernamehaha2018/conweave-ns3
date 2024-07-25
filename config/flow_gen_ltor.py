import random

host_id_start=0
host_id_end=128

start_time=2
flow_perio=3

flow_per_host=3
flow_size=20*1024*1024 #20MB

flow_lst=[]

for src in range(host_id_start,host_id_end//2):
    for flow_count in range(flow_per_host):
        dst=random.randint(host_id_start//2,host_id_end-1)
        while(dst==src):
            dst=random.randint(host_id_start//2,host_id_end-1)
        flow_lst.append([src,dst])

print(len(flow_lst))
for [src,dest] in flow_lst:
    print(f"{src} {dest} {flow_perio} {flow_size} {start_time}")