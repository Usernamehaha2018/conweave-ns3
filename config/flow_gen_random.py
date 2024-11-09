import random

host_id_start=0
host_id_end=128

start_time=2
flow_perio=3

flow_per_host=1
flow_size=200*1024*1024 #20MB

flow_lst=[]
# simulate ring all reduce like traffic
host_seen = {}

src = host_id_start
while len(host_seen) != host_id_end-1:
    dst = src
    while dst in host_seen or dst == src:
        dst = random.randint(host_id_start + 1, host_id_end-1)
    host_seen[dst] = 1
    flow_lst.append([src,dst,flow_size])
    src = dst
flow_lst.append([src,host_id_start,flow_size])
    
flow_lst.append([0,9,100]) #for fct analysis

print(len(flow_lst))
for [src,dest,flow_size] in flow_lst:
    print(f"{src} {dest} {flow_perio} {flow_size} {start_time}")


