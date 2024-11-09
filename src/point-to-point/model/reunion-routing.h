#pragma once

#include <arpa/inet.h>

#include <map>
#include <queue>
#include <unordered_map>
#include <vector>
#include <set>

#include "ns3/address.h"
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/net-device.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/settings.h"
#include "ns3/simulator.h"
#include "ns3/tag.h"

#include "ns3/letflow-routing.h"

namespace ns3 {

const uint32_t REUNION_NULL = 114514;

struct ReunionFlowlet{
    Time _activeTime;     // to check creating a new flowlet
    Time _activatedTime;  // start time of new flowlet

    Time _lastReroute; //for reroute for subflowlet

    uint32_t _PathId;     // current pathId
    uint32_t _nPackets;   // for debugging
    uint32_t _reroute_cnt; // for analyzing

};

class ReunionTag : public Tag {
   public:
    ReunionTag();
    ~ReunionTag();
    static TypeId GetTypeId(void);
    void SetPathId(uint32_t pathId);
    uint32_t GetPathId(void) const;
    void SetHopCount(uint32_t hopCount);
    uint32_t GetHopCount(void) const;
    virtual TypeId GetInstanceTypeId(void) const;
    virtual uint32_t GetSerializedSize(void) const;
    virtual void Serialize(TagBuffer i) const;
    virtual void Deserialize(TagBuffer i);
    virtual void Print(std::ostream& os) const;

   private:
    uint32_t m_pathId;    // forward
    uint32_t m_hopCount;  // hopCount to get outPort
};

class ReunionRouting : public Object {
    friend class SwitchMmu;
    friend class SwitchNode;

   public:
    ReunionRouting();

    uint64_t GetQpKey(uint32_t dip, uint16_t sport, uint16_t dport, uint16_t pg);  
    uint32_t GetOutPortFromPath(const uint32_t& path, const uint32_t& hopCount);
    uint32_t GetRandomPath(uint32_t dstTorId);
    Time _FlowletTimeout;
    Time _SubFlowletGap;

    uint32_t RouteInput(Ptr<Packet> p, CustomHeader ch);
    ReunionFlowlet* GetFlowlet(uint64_t Qpkey,CustomHeader ch,uint32_t hopcount);

    EventId m_agingEvent;

    // 统计数据
    uint32_t reroute_count;
    Time m_agingTime;

    EventId m_stat_agingEvent;
    Time m_stat_agingTime;
    std::map<uint32_t,uint32_t> _PortTransmit;
    uint32_t _HighUtilCount;
    uint32_t _LowUtilCount;
    void AgingEvent();
    void PortAgingEvent();

    EventId m_calculateUtilization;
    void calculateUtilization();

    std::set<uint32_t> _HighUtilizedPort;
    std::set<uint32_t> _LowUtilizedPort;
    std::map<uint32_t,uint32_t>_PortRerouteCnt;
    std::map<uint64_t,ReunionFlowlet*> _QpkeyToFlowlet;
    std::map<uint32_t,std::set<uint32_t>> _ReunionRouteTable;

    bool m_isToR;          // is ToR (leaf)
    uint32_t m_switch_id;  // switch's nodeID
    void SetSwitchInfo(bool isToR, uint32_t switch_id);
    uint32_t GetSwitchId();
    
};

}  // namespace ns3