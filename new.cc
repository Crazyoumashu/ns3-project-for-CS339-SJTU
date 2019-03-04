#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

#include "ns3/tag.h"
#include "ns3/simulator.h"
#include "ns3/string.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/test.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/simple-net-device-helper.h"
#include "ns3/simple-net-device.h"
#include <time.h>
using namespace ns3;
using namespace std;
class routerList
{
	public:
	int nHosts;
	int nRouters_0;
	int nRouters_1;
	int connectHosts[5];
	int connectRouters_0[5];
	int connectRouters_1[5];
};
vector<routerList> RouterLists(50);
vector<NetDeviceContainer> devicesHosts(50);
vector<NetDeviceContainer> devicesRouters(50);
int uid_last=0;
int uid_now=0;
int hostPairs=0;
double sendTime[20000];
double receiveTime[20000];
int kdevice[15];
int counter = 0;
NS_LOG_COMPONENT_DEFINE ("test");
static void
configPacket (Ptr<const Packet> p,const Address &src)
{
	uid_now=p->GetUid();
	if(uid_now>uid_last&&uid_now-uid_last<20000)
  	receiveTime[uid_now-uid_last] = Simulator::Now().GetSeconds();
}
static void
sendCP (Ptr<const Packet> p)
{
    int uid = p->GetUid();
    if(uid>uid_last&&uid-uid_last<20000)
  	sendTime[uid-uid_last] = Simulator::Now().GetSeconds();
}

void getmaxQ()
{
    int maxlen = 0;
    //int Maxlen = 0;
    int minlen = 999999;
	//uint32_t len=0;
  for(int i=0;i<30;i++)
  {
	maxlen=0;
	for(int k=0; k<RouterLists[i].nHosts; k++)
	{
			PointerValue ptr;
			devicesHosts[RouterLists[i].connectHosts[k]].Get(0)->GetAttribute("TxQueue",ptr);
    		Ptr<Queue> txQueue = ptr.Get<Queue>();
			kdevice[counter] = txQueue->GetNPackets();
			counter++;
	 }
	for(int k=0; k<RouterLists[i].nRouters_0; k++)
	{
			PointerValue ptr;
			devicesRouters[RouterLists[i].connectRouters_0[k]].Get(0)->GetAttribute("TxQueue",ptr);
    		Ptr<Queue> txQueue = ptr.Get<Queue>();
			kdevice[counter] = txQueue->GetNPackets();
			counter++;
	}
	for(int k=0; k<RouterLists[i].nRouters_1; k++)
	{
			PointerValue ptr;
			devicesRouters[RouterLists[i].connectRouters_1[k]].Get(1)->GetAttribute("TxQueue",ptr);
                Ptr<Queue> txQueue = ptr.Get<Queue>();
    		kdevice[counter] = txQueue->GetNPackets();
			counter++;
	}
	for(int k =0; k<counter; k++)
	{
	    if(kdevice[k]>maxlen)
	    maxlen = kdevice[k];
	}
    counter = 0;
    //len = maxlen;
    //if(maxlen>Maxlen) Maxlen=maxlen;
     if(maxlen<minlen&&maxlen!=0) minlen = maxlen;
  }
    //cout <<Simulator::Now().GetSeconds()<<"\t" << Maxlen << endl;
    cout <<Simulator::Now().GetSeconds()<<"\t" << minlen << endl;
}

void calThroughput()
{
	int throughput_now;
if(uid_now-uid_last>0)
{
 throughput_now = (uid_now-uid_last)/hostPairs;
}
else
{
throughput_now = (uid_last-uid_now)/hostPairs;
}
	cout <<Simulator::Now().GetSeconds()<<"\t" <<throughput_now<<endl;
	uid_last=uid_now;
}
void calDelay()
{
	double delayMin_now=9;
        if(uid_now-uid_last>0)
{
	for(int i=1;i<uid_now-uid_last;i++)
	{
		double thisDelay=receiveTime[i]-sendTime[i];
		if(thisDelay > 0)
		{
			if(delayMin_now>thisDelay)
				delayMin_now=thisDelay;
		}

	}
        if(delayMin_now == 9)
{
        cout <<Simulator::Now().GetSeconds()<<"\t" <<0.008768<<endl;
}
        else
       {
	cout <<Simulator::Now().GetSeconds()<<"\t" <<delayMin_now<<endl;
	uid_last=uid_now;
       }
}
else cout <<Simulator::Now().GetSeconds()<<"\t" <<0.006576<<endl;
}
int randlist[50];
void randC(int N)
{
    int *arr = (int*)malloc(N*sizeof(int));
    int count = 0;
    memset(arr,0,N*sizeof(int));
    //srand(time(NULL));
    while(count<N)
    {
        int val = rand()%N;
        if (!arr[val])
        {
            randlist[count] = val;
            arr[val]=1;
            ++count;
        }
    }
    free(arr);
    arr = NULL;
}
void
SET()
{
   RouterLists[0].nHosts =  2;
   RouterLists[0].connectHosts[0] = 0;
   RouterLists[0].connectHosts[1] = 1;
   RouterLists[0].nRouters_0 = 0;
   RouterLists[0].nRouters_1 = 3;
   RouterLists[0].connectRouters_1[0] = 0;
   RouterLists[0].connectRouters_1[1] = 1;
   RouterLists[0].connectRouters_1[2] = 2;

   RouterLists[1].nHosts =  2;
   RouterLists[1].connectHosts[0] = 2;
   RouterLists[1].connectHosts[1] = 3;
   RouterLists[1].nRouters_0 = 1;
   RouterLists[1].connectRouters_0[0] = 0;
   RouterLists[1].nRouters_1 = 1;
   RouterLists[1].connectRouters_1[0] = 3;

   RouterLists[2].nHosts =  2;
   RouterLists[2].connectHosts[0] = 4;
   RouterLists[2].connectHosts[1] = 5;
   RouterLists[2].nRouters_0 = 1;
   RouterLists[2].connectRouters_0[0] = 3;
   RouterLists[2].nRouters_1 = 2;
   RouterLists[2].connectRouters_1[0] = 4;
   RouterLists[2].connectRouters_1[1] = 5;

   RouterLists[3].nHosts =  2;
   RouterLists[3].connectHosts[0] = 6;
   RouterLists[3].connectHosts[1] = 7;
   RouterLists[3].nRouters_0 = 2;
   RouterLists[3].connectRouters_0[0] = 1;
   RouterLists[3].connectRouters_0[1] = 4;
   RouterLists[3].nRouters_1 = 1;
   RouterLists[3].connectRouters_1[0] = 6;

   RouterLists[4].nHosts = 2;
   RouterLists[4].connectHosts[0] = 8;
   RouterLists[4].connectHosts[1] = 9;
   RouterLists[4].nRouters_0 = 1;
   RouterLists[4].connectRouters_0[0] = 2;
   RouterLists[4].nRouters_1 = 2;
   RouterLists[4].connectRouters_1[0] = 7;
   RouterLists[4].connectRouters_1[1] = 8;

   RouterLists[5].nHosts = 1;
   RouterLists[5].connectHosts[0] = 10;
   RouterLists[5].nRouters_0 = 2;
   RouterLists[5].connectRouters_0[0] = 6;
   RouterLists[5].connectRouters_0[1] = 7;
   RouterLists[5].nRouters_1 = 1;
   RouterLists[5].connectRouters_1[0] = 9;

   RouterLists[6].nHosts = 0;
   RouterLists[6].nRouters_0 = 1;
   RouterLists[6].connectRouters_0[0] = 9;
   RouterLists[6].nRouters_1 = 2;
   RouterLists[6].connectRouters_1[0] = 10;
   RouterLists[6].connectRouters_1[1] = 11;

   RouterLists[7].nHosts = 3;
   RouterLists[7].connectHosts[0] = 11;
   RouterLists[7].connectHosts[1] = 12;
   RouterLists[7].connectHosts[2] = 13;
   RouterLists[7].nRouters_0 = 2;
   RouterLists[7].connectRouters_0[0] = 5;
   RouterLists[7].connectRouters_0[1] = 10;
   RouterLists[7].nRouters_1 = 1;
   RouterLists[7].connectRouters_1[0] = 12;

   RouterLists[8].nHosts = 3;
   RouterLists[8].connectHosts[0] = 14;
   RouterLists[8].connectHosts[1] = 15;
   RouterLists[8].connectHosts[2] = 16;
   RouterLists[8].nRouters_0 = 1;
   RouterLists[8].connectRouters_0[0] = 8;
   RouterLists[8].nRouters_1 = 2;
   RouterLists[8].connectRouters_1[0] = 13;
   RouterLists[8].connectRouters_1[1] = 14;

   RouterLists[9].nHosts = 1;
   RouterLists[9].connectHosts[0] = 18;
   RouterLists[9].nRouters_0 = 1;
   RouterLists[9].connectRouters_0[0] = 12;
   RouterLists[9].nRouters_1 = 2;
   RouterLists[9].connectRouters_1[0] = 15;
   RouterLists[9].connectRouters_1[1] = 16;

   RouterLists[10].nHosts = 2;
   RouterLists[10].connectHosts[0] = 19;
   RouterLists[10].connectHosts[1] = 20;
   RouterLists[10].nRouters_0 = 1;
   RouterLists[10].connectRouters_0[0] = 13;
   RouterLists[10].nRouters_1 = 3;
   RouterLists[10].connectRouters_1[0] = 18;
   RouterLists[10].connectRouters_1[1] = 19;
   RouterLists[10].connectRouters_1[2] = 17;

   RouterLists[11].nHosts = 1;
   RouterLists[11].connectHosts[0] = 21;
   RouterLists[11].nRouters_0 = 1;
   RouterLists[11].connectRouters_0[0] = 14;
   RouterLists[11].nRouters_1 = 2;
   RouterLists[11].connectRouters_1[0] = 20;
   RouterLists[11].connectRouters_1[1] = 21;

   RouterLists[12].nHosts = 1;
   RouterLists[12].connectHosts[0] = 17;
   RouterLists[12].nRouters_0 = 3;
   RouterLists[12].connectRouters_0[0] = 11;
   RouterLists[12].connectRouters_0[1] = 15;
   RouterLists[12].connectRouters_0[2] = 20;
   RouterLists[12].nRouters_1 = 1;
   RouterLists[12].connectRouters_1[0] = 22;

   RouterLists[13].nHosts = 2;
   RouterLists[13].connectHosts[0] = 22;
   RouterLists[13].connectHosts[1] = 23;
   RouterLists[13].nRouters_0 = 2;
   RouterLists[13].connectRouters_0[0] = 19;
   RouterLists[13].connectRouters_0[1] = 21;
   RouterLists[13].nRouters_1 = 2;
   RouterLists[13].connectRouters_1[0] = 23;
   RouterLists[13].connectRouters_1[1] = 24;

   RouterLists[14].nHosts = 1;
   RouterLists[14].connectHosts[0] = 24;
   RouterLists[14].nRouters_0 = 1;
   RouterLists[14].connectRouters_0[0] = 23;
   RouterLists[14].nRouters_1 = 0;

   RouterLists[15].nHosts = 3;
   RouterLists[15].connectHosts[0] = 25;
   RouterLists[15].connectHosts[1] = 26;
   RouterLists[15].connectHosts[2] = 27;
   RouterLists[15].nRouters_0 = 1;
   RouterLists[15].connectRouters_0[0] = 16;
   RouterLists[15].nRouters_1 = 1;
   RouterLists[15].connectRouters_1[0] = 25;

   RouterLists[16].nHosts = 3;
   RouterLists[16].connectHosts[0] = 28;
   RouterLists[16].connectHosts[1] = 29;
   RouterLists[16].connectHosts[2] = 30;
   RouterLists[16].nRouters_0 = 1;
   RouterLists[16].connectRouters_0[0] = 24;
   RouterLists[16].nRouters_1 = 2;
   RouterLists[16].connectRouters_1[0] = 26;
   RouterLists[16].connectRouters_1[1] = 27;

   RouterLists[17].nHosts = 1;
   RouterLists[17].connectHosts[0] = 31;
   RouterLists[17].nRouters_0 = 1;
   RouterLists[17].connectRouters_0[0] = 26;
   RouterLists[17].nRouters_1 = 2;
   RouterLists[17].connectRouters_1[0] = 28;
   RouterLists[17].connectRouters_1[1] = 29;

   RouterLists[18].nHosts = 0;
   RouterLists[18].connectHosts[0] = 32;
   RouterLists[18].nRouters_0 = 2;
   RouterLists[18].connectRouters_0[0] = 27;
   RouterLists[18].connectRouters_0[1] = 28;
   RouterLists[18].nRouters_1 = 2;
   RouterLists[18].connectRouters_1[0] = 30;
   RouterLists[18].connectRouters_1[1] = 31;

   RouterLists[19].nHosts = 1;
   RouterLists[19].connectHosts[0] = 33;
   RouterLists[19].nRouters_0 = 3;
   RouterLists[19].connectRouters_0[0] = 22;
   RouterLists[19].connectRouters_0[1] = 25;
   RouterLists[19].connectRouters_0[2] = 30;
   RouterLists[19].nRouters_1 = 3;
   RouterLists[19].connectRouters_1[0] = 32;
   RouterLists[19].connectRouters_1[1] = 34;
   RouterLists[19].connectRouters_1[2] = 33;

   RouterLists[20].nHosts = 4;
   RouterLists[20].connectHosts[0] = 34;
   RouterLists[20].connectHosts[1] = 35;
   RouterLists[20].connectHosts[2] = 36;
   RouterLists[20].connectHosts[3] = 37;
   RouterLists[20].nRouters_0 = 1;
   RouterLists[20].connectRouters_0[0] = 32;
   RouterLists[20].nRouters_1 = 1;
   RouterLists[20].connectRouters_1[0] = 35;

   RouterLists[21].nHosts = 1;
   RouterLists[21].connectHosts[0] = 38;
   RouterLists[21].nRouters_0 = 1;
   RouterLists[21].connectRouters_0[0] = 17;
   RouterLists[21].nRouters_1 = 3;
   RouterLists[21].connectRouters_1[0] = 36;
   RouterLists[21].connectRouters_1[1] = 37;
   RouterLists[21].connectRouters_1[2] = 38;

   RouterLists[22].nHosts = 0;
   RouterLists[22].nRouters_0 = 3;
   RouterLists[22].connectRouters_0[0] = 29;
   RouterLists[22].connectRouters_0[1] = 31;
   RouterLists[22].connectRouters_0[2] = 36;
   RouterLists[22].nRouters_1 = 2;
   RouterLists[22].connectRouters_1[0] = 39;
   RouterLists[22].connectRouters_1[1] = 40;

   RouterLists[23].nHosts = 1;
   RouterLists[23].connectHosts[0] = 47;
   RouterLists[23].nRouters_0 = 2;
   RouterLists[23].connectRouters_0[0] = 34;
   RouterLists[23].connectRouters_0[1] = 39;
   RouterLists[23].nRouters_1 = 2;
   RouterLists[23].connectRouters_1[0] = 41;
   RouterLists[23].connectRouters_1[1] = 42;

   RouterLists[24].nHosts = 1;
   RouterLists[24].connectHosts[0] = 49;
   RouterLists[24].nRouters_0 = 3;
   RouterLists[24].connectRouters_0[0] = 37;
   RouterLists[24].connectRouters_0[1] = 40;
   RouterLists[24].connectRouters_0[2] = 41;
   RouterLists[24].nRouters_1 = 1;
   RouterLists[24].connectRouters_1[0] = 43;

   RouterLists[25].nHosts = 1;
   RouterLists[25].connectHosts[0] = 39;
   RouterLists[25].nRouters_0 = 1;
   RouterLists[25].connectRouters_0[0] = 33;
   RouterLists[25].nRouters_1 = 2;
   RouterLists[25].connectRouters_1[0] = 44;
   RouterLists[25].connectRouters_1[1] = 45;

   RouterLists[26].nHosts = 1;
   RouterLists[26].connectHosts[0] = 48;
   RouterLists[26].nRouters_0 = 1;
   RouterLists[26].connectRouters_0[0] = 43;
   RouterLists[26].nRouters_1 = 2;
   RouterLists[26].connectRouters_1[0] = 46;
   RouterLists[26].connectRouters_1[1] = 47;

   RouterLists[27].nHosts = 1;
   RouterLists[27].connectHosts[0] = 43;
   RouterLists[27].nRouters_0 = 3;
   RouterLists[27].connectRouters_0[0] = 42;
   RouterLists[27].connectRouters_0[1] = 44;
   RouterLists[27].connectRouters_0[2] = 46;
   RouterLists[27].nRouters_1 = 1;
   RouterLists[27].connectRouters_1[0] = 48;

   RouterLists[28].nHosts = 3;
   RouterLists[28].connectHosts[0] = 44;
   RouterLists[28].connectHosts[1] = 45;
   RouterLists[28].connectHosts[2] = 46;
   RouterLists[28].nRouters_0 = 3;
   RouterLists[28].connectRouters_0[0] = 35;
   RouterLists[28].connectRouters_0[1] = 45;
   RouterLists[28].connectRouters_0[2] = 48;
   RouterLists[28].nRouters_1 = 1;
   RouterLists[28].connectRouters_1[0] = 49;

   RouterLists[29].nHosts = 0;
   RouterLists[29].connectHosts[0] = 40;
   RouterLists[29].connectHosts[1] = 41;
   RouterLists[29].connectHosts[2] = 42;
   RouterLists[29].nRouters_0 = 3;
   RouterLists[29].connectRouters_0[0] = 38;
   RouterLists[29].connectRouters_0[1] = 47;
   RouterLists[29].connectRouters_0[2] = 49;
   RouterLists[29].nRouters_1 = 0;
}
int
main (int argc, char *argv[])
{
    srand((unsigned)time(NULL));
    SET();
    Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
    Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("10Mb/s"));//可算得每包间隔3.75ms
    NS_LOG_INFO("Create nodes");
    NodeContainer Host;
    NodeContainer Rout;
    Host.Create(50);
    Rout.Create(30);
    vector<NodeContainer> nodeAdjacencyList(100);
    nodeAdjacencyList[0] = NodeContainer(Host.Get(0), Rout.Get(0));//h1r1
    nodeAdjacencyList[1] = NodeContainer(Host.Get(1), Rout.Get(0));//h2r1
    nodeAdjacencyList[2] = NodeContainer(Host.Get(2), Rout.Get(1));//h3r2
    nodeAdjacencyList[3] = NodeContainer(Host.Get(3), Rout.Get(1));//h4r2
    nodeAdjacencyList[4] = NodeContainer(Host.Get(4), Rout.Get(2));//h5r3
    nodeAdjacencyList[5] = NodeContainer(Host.Get(5), Rout.Get(2));//h6r3
    nodeAdjacencyList[6] = NodeContainer(Host.Get(6), Rout.Get(3));//h7r4
    nodeAdjacencyList[7] = NodeContainer(Host.Get(7), Rout.Get(3));//h8r4
    nodeAdjacencyList[8] = NodeContainer(Host.Get(8), Rout.Get(4));//h9r5
    nodeAdjacencyList[9] = NodeContainer(Host.Get(9), Rout.Get(4));//h10r5
    nodeAdjacencyList[10] = NodeContainer(Host.Get(10), Rout.Get(5));//h11r6
    nodeAdjacencyList[11] = NodeContainer(Host.Get(11), Rout.Get(7));//h12r8
    nodeAdjacencyList[12] = NodeContainer(Host.Get(12), Rout.Get(7));//h13r8
    nodeAdjacencyList[13] = NodeContainer(Host.Get(13), Rout.Get(7));//h14r8
    nodeAdjacencyList[14] = NodeContainer(Host.Get(14), Rout.Get(8));
    nodeAdjacencyList[15] = NodeContainer(Host.Get(15), Rout.Get(8));
    nodeAdjacencyList[16] = NodeContainer(Host.Get(16), Rout.Get(8));
    nodeAdjacencyList[17] = NodeContainer(Host.Get(17), Rout.Get(12));
    nodeAdjacencyList[18] = NodeContainer(Host.Get(18), Rout.Get(9));
    nodeAdjacencyList[19] = NodeContainer(Host.Get(19), Rout.Get(10));
    nodeAdjacencyList[20] = NodeContainer(Host.Get(20), Rout.Get(10));
    nodeAdjacencyList[21] = NodeContainer(Host.Get(21), Rout.Get(11));
    nodeAdjacencyList[22] = NodeContainer(Host.Get(22), Rout.Get(13));
    nodeAdjacencyList[23] = NodeContainer(Host.Get(23), Rout.Get(13));
    nodeAdjacencyList[24] = NodeContainer(Host.Get(24), Rout.Get(14));
    nodeAdjacencyList[25] = NodeContainer(Host.Get(25), Rout.Get(15));
    nodeAdjacencyList[26] = NodeContainer(Host.Get(26), Rout.Get(15));
    nodeAdjacencyList[27] = NodeContainer(Host.Get(27), Rout.Get(15));
    nodeAdjacencyList[28] = NodeContainer(Host.Get(28), Rout.Get(16));
    nodeAdjacencyList[29] = NodeContainer(Host.Get(29), Rout.Get(16));
    nodeAdjacencyList[30] = NodeContainer(Host.Get(30), Rout.Get(16));
    nodeAdjacencyList[31] = NodeContainer(Host.Get(31), Rout.Get(17));
    nodeAdjacencyList[32] = NodeContainer(Host.Get(32), Rout.Get(18));
    nodeAdjacencyList[33] = NodeContainer(Host.Get(33), Rout.Get(19));
    nodeAdjacencyList[34] = NodeContainer(Host.Get(34), Rout.Get(20));
    nodeAdjacencyList[35] = NodeContainer(Host.Get(35), Rout.Get(20));
    nodeAdjacencyList[36] = NodeContainer(Host.Get(36), Rout.Get(20));
    nodeAdjacencyList[37] = NodeContainer(Host.Get(37), Rout.Get(20));
    nodeAdjacencyList[38] = NodeContainer(Host.Get(38), Rout.Get(21));
    nodeAdjacencyList[39] = NodeContainer(Host.Get(39), Rout.Get(25));
    nodeAdjacencyList[40] = NodeContainer(Host.Get(40), Rout.Get(29));
    nodeAdjacencyList[41] = NodeContainer(Host.Get(41), Rout.Get(29));
    nodeAdjacencyList[42] = NodeContainer(Host.Get(42), Rout.Get(29));
    nodeAdjacencyList[43] = NodeContainer(Host.Get(43), Rout.Get(27));
    nodeAdjacencyList[44] = NodeContainer(Host.Get(44), Rout.Get(28));
    nodeAdjacencyList[45] = NodeContainer(Host.Get(45), Rout.Get(28));
    nodeAdjacencyList[46] = NodeContainer(Host.Get(46), Rout.Get(28));
    nodeAdjacencyList[47] = NodeContainer(Host.Get(47), Rout.Get(23));
    nodeAdjacencyList[48] = NodeContainer(Host.Get(48), Rout.Get(26));
    nodeAdjacencyList[49] = NodeContainer(Host.Get(49), Rout.Get(24));
    nodeAdjacencyList[50] = NodeContainer(Rout.Get(0), Rout.Get(1));
    nodeAdjacencyList[51] = NodeContainer(Rout.Get(0), Rout.Get(3));
    nodeAdjacencyList[52] = NodeContainer(Rout.Get(0), Rout.Get(4));
    nodeAdjacencyList[53] = NodeContainer(Rout.Get(1), Rout.Get(2));
    nodeAdjacencyList[54] = NodeContainer(Rout.Get(2), Rout.Get(3));
    nodeAdjacencyList[55] = NodeContainer(Rout.Get(2), Rout.Get(7));
    nodeAdjacencyList[56] = NodeContainer(Rout.Get(3), Rout.Get(5));
    nodeAdjacencyList[57] = NodeContainer(Rout.Get(4), Rout.Get(5));
    nodeAdjacencyList[58] = NodeContainer(Rout.Get(4), Rout.Get(8));
    nodeAdjacencyList[59] = NodeContainer(Rout.Get(5), Rout.Get(6));
    nodeAdjacencyList[60] = NodeContainer(Rout.Get(6), Rout.Get(7));
    nodeAdjacencyList[61] = NodeContainer(Rout.Get(6), Rout.Get(12));
    nodeAdjacencyList[62] = NodeContainer(Rout.Get(7), Rout.Get(9));
    nodeAdjacencyList[63] = NodeContainer(Rout.Get(8), Rout.Get(10));
    nodeAdjacencyList[64] = NodeContainer(Rout.Get(8), Rout.Get(11));
    nodeAdjacencyList[65] = NodeContainer(Rout.Get(9), Rout.Get(12));
    nodeAdjacencyList[66] = NodeContainer(Rout.Get(9), Rout.Get(15));
    nodeAdjacencyList[67] = NodeContainer(Rout.Get(10), Rout.Get(11));
    nodeAdjacencyList[68] = NodeContainer(Rout.Get(10), Rout.Get(13));
    nodeAdjacencyList[69] = NodeContainer(Rout.Get(10), Rout.Get(21));
    nodeAdjacencyList[70] = NodeContainer(Rout.Get(11), Rout.Get(12));
    nodeAdjacencyList[71] = NodeContainer(Rout.Get(11), Rout.Get(13));
    nodeAdjacencyList[72] = NodeContainer(Rout.Get(12), Rout.Get(19));
    nodeAdjacencyList[73] = NodeContainer(Rout.Get(13), Rout.Get(14));
    nodeAdjacencyList[74] = NodeContainer(Rout.Get(13), Rout.Get(16));
    nodeAdjacencyList[75] = NodeContainer(Rout.Get(15), Rout.Get(19));
    nodeAdjacencyList[76] = NodeContainer(Rout.Get(16), Rout.Get(17));
    nodeAdjacencyList[77] = NodeContainer(Rout.Get(16), Rout.Get(18));
    nodeAdjacencyList[78] = NodeContainer(Rout.Get(17), Rout.Get(18));
    nodeAdjacencyList[79] = NodeContainer(Rout.Get(17), Rout.Get(22));
    nodeAdjacencyList[80] = NodeContainer(Rout.Get(18), Rout.Get(19));
    nodeAdjacencyList[81] = NodeContainer(Rout.Get(18), Rout.Get(22));
    nodeAdjacencyList[82] = NodeContainer(Rout.Get(19), Rout.Get(20));
    nodeAdjacencyList[83] = NodeContainer(Rout.Get(19), Rout.Get(23));
    nodeAdjacencyList[84] = NodeContainer(Rout.Get(19), Rout.Get(25));
    nodeAdjacencyList[85] = NodeContainer(Rout.Get(20), Rout.Get(28));
    nodeAdjacencyList[86] = NodeContainer(Rout.Get(21), Rout.Get(22));
    nodeAdjacencyList[87] = NodeContainer(Rout.Get(21), Rout.Get(24));
    nodeAdjacencyList[88] = NodeContainer(Rout.Get(21), Rout.Get(29));
    nodeAdjacencyList[89] = NodeContainer(Rout.Get(22), Rout.Get(23));
    nodeAdjacencyList[90] = NodeContainer(Rout.Get(22), Rout.Get(24));
    nodeAdjacencyList[91] = NodeContainer(Rout.Get(23), Rout.Get(24));
    nodeAdjacencyList[92] = NodeContainer(Rout.Get(23), Rout.Get(27));
    nodeAdjacencyList[93] = NodeContainer(Rout.Get(24), Rout.Get(26));
    nodeAdjacencyList[94] = NodeContainer(Rout.Get(25), Rout.Get(27));
    nodeAdjacencyList[95] = NodeContainer(Rout.Get(25), Rout.Get(28));
    nodeAdjacencyList[96] = NodeContainer(Rout.Get(26), Rout.Get(27));
    nodeAdjacencyList[97] = NodeContainer(Rout.Get(26), Rout.Get(29));
    nodeAdjacencyList[98] = NodeContainer(Rout.Get(27), Rout.Get(28));
    nodeAdjacencyList[99] = NodeContainer(Rout.Get(28), Rout.Get(29));

    InternetStackHelper internet1;
    internet1.Install(Host);
    //Enable OLSR
    NS_LOG_INFO("Enabling new Routing");
    /*OlsrHelper oslr;
    Ipv4StaticRoutingHelper staticRouting;
    Ipv4ListRoutingHelper list;
    list.Add(staticRouting, 0);
    list.Add(oslr,10);*/
    InternetStackHelper internet2;
    //internet2.SetRoutingHelper(list);
    internet2.Install(Rout);
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
    p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", StringValue("10000"));
    p2p.SetDeviceAttribute("InterframeGap", StringValue("333333ns"));


    for(int i = 0; i<50; i++)
    {
        devicesHosts[i] = p2p.Install(nodeAdjacencyList[i]);
    }
    for(int i = 50; i<100; i++)
    {
        devicesRouters[i-50] = p2p.Install(nodeAdjacencyList[i]);
    }
    Ptr<RateErrorModel>em=CreateObject<RateErrorModel> ();
    em->SetAttribute("ErrorRate",DoubleValue(0.00001));
    for(int i = 0; i<50; i++)
    {
        devicesHosts[i].Get(0)->SetAttribute("ReceiveErrorModel",PointerValue (em));
    }
    //add IP Addresses
    Ipv4AddressHelper addressHosts;
	vector<Ipv4InterfaceContainer> interfacesHosts(50);
	addressHosts.SetBase("140.1.1.0", "255.255.255.0");
	interfacesHosts[0] = addressHosts.Assign(devicesHosts[0]);
	interfacesHosts[1] = addressHosts.Assign(devicesHosts[1]);
	addressHosts.SetBase("140.1.2.0", "255.255.255.0");
	interfacesHosts[2] = addressHosts.Assign(devicesHosts[2]);
	interfacesHosts[3] = addressHosts.Assign(devicesHosts[3]);
	addressHosts.SetBase("140.1.3.0", "255.255.255.0");
	interfacesHosts[4] = addressHosts.Assign(devicesHosts[4]);
	interfacesHosts[5] = addressHosts.Assign(devicesHosts[5]);
	addressHosts.SetBase("140.1.4.0", "255.255.255.0");
	interfacesHosts[6] = addressHosts.Assign(devicesHosts[6]);
	interfacesHosts[7] = addressHosts.Assign(devicesHosts[7]);
	addressHosts.SetBase("140.1.5.0", "255.255.255.0");
	interfacesHosts[8] = addressHosts.Assign(devicesHosts[8]);
	interfacesHosts[9] = addressHosts.Assign(devicesHosts[9]);
	addressHosts.SetBase("140.1.6.0", "255.255.255.0");
	interfacesHosts[10] = addressHosts.Assign(devicesHosts[10]);
	addressHosts.SetBase("140.1.8.0", "255.255.255.0");
	interfacesHosts[11] = addressHosts.Assign(devicesHosts[11]);
	interfacesHosts[12] = addressHosts.Assign(devicesHosts[12]);
	interfacesHosts[13] = addressHosts.Assign(devicesHosts[13]);
	addressHosts.SetBase("140.1.9.0", "255.255.255.0");
	interfacesHosts[14] = addressHosts.Assign(devicesHosts[14]);
	interfacesHosts[15] = addressHosts.Assign(devicesHosts[15]);
	interfacesHosts[16] = addressHosts.Assign(devicesHosts[16]);
	addressHosts.SetBase("140.1.10.0", "255.255.255.0");
	interfacesHosts[18] = addressHosts.Assign(devicesHosts[18]);
	addressHosts.SetBase("140.1.11.0", "255.255.255.0");
	interfacesHosts[19] = addressHosts.Assign(devicesHosts[19]);
	interfacesHosts[20] = addressHosts.Assign(devicesHosts[20]);
	addressHosts.SetBase("140.1.12.0", "255.255.255.0");
	interfacesHosts[21] = addressHosts.Assign(devicesHosts[21]);
	addressHosts.SetBase("140.1.13.0", "255.255.255.0");
	interfacesHosts[17] = addressHosts.Assign(devicesHosts[17]);
	addressHosts.SetBase("140.1.14.0", "255.255.255.0");
	interfacesHosts[22] = addressHosts.Assign(devicesHosts[22]);
	interfacesHosts[23] = addressHosts.Assign(devicesHosts[23]);
	addressHosts.SetBase("140.1.15.0", "255.255.255.0");
	interfacesHosts[24] = addressHosts.Assign(devicesHosts[24]);
	addressHosts.SetBase("140.1.16.0", "255.255.255.0");
	interfacesHosts[25] = addressHosts.Assign(devicesHosts[25]);
	interfacesHosts[26] = addressHosts.Assign(devicesHosts[26]);
	interfacesHosts[27] = addressHosts.Assign(devicesHosts[27]);
	addressHosts.SetBase("140.1.17.0", "255.255.255.0");
	interfacesHosts[28] = addressHosts.Assign(devicesHosts[28]);
	interfacesHosts[29] = addressHosts.Assign(devicesHosts[29]);
	interfacesHosts[30] = addressHosts.Assign(devicesHosts[30]);
	addressHosts.SetBase("140.1.18.0", "255.255.255.0");
	interfacesHosts[31] = addressHosts.Assign(devicesHosts[31]);
	addressHosts.SetBase("140.1.19.0", "255.255.255.0");
	interfacesHosts[32] = addressHosts.Assign(devicesHosts[32]);
	addressHosts.SetBase("140.1.20.0", "255.255.255.0");
	interfacesHosts[33] = addressHosts.Assign(devicesHosts[33]);
	addressHosts.SetBase("140.1.21.0", "255.255.255.0");
	interfacesHosts[34] = addressHosts.Assign(devicesHosts[34]);
	interfacesHosts[35] = addressHosts.Assign(devicesHosts[35]);
	interfacesHosts[36] = addressHosts.Assign(devicesHosts[36]);
	interfacesHosts[37] = addressHosts.Assign(devicesHosts[37]);
	addressHosts.SetBase("140.1.22.0", "255.255.255.0");
	interfacesHosts[38] = addressHosts.Assign(devicesHosts[38]);
	addressHosts.SetBase("140.1.24.0", "255.255.255.0");
	interfacesHosts[47] = addressHosts.Assign(devicesHosts[47]);
	addressHosts.SetBase("140.1.25.0", "255.255.255.0");
	interfacesHosts[49] = addressHosts.Assign(devicesHosts[49]);
	addressHosts.SetBase("140.1.26.0", "255.255.255.0");
	interfacesHosts[39] = addressHosts.Assign(devicesHosts[39]);
	addressHosts.SetBase("140.1.27.0", "255.255.255.0");
	interfacesHosts[48] = addressHosts.Assign(devicesHosts[48]);
	addressHosts.SetBase("140.1.28.0", "255.255.255.0");
	interfacesHosts[43] = addressHosts.Assign(devicesHosts[43]);
	addressHosts.SetBase("140.1.29.0", "255.255.255.0");
	interfacesHosts[44] = addressHosts.Assign(devicesHosts[44]);
	interfacesHosts[45] = addressHosts.Assign(devicesHosts[45]);
	interfacesHosts[46] = addressHosts.Assign(devicesHosts[46]);
	addressHosts.SetBase("140.1.30.0", "255.255.255.0");
	interfacesHosts[40] = addressHosts.Assign(devicesHosts[40]);
	interfacesHosts[41] = addressHosts.Assign(devicesHosts[41]);
	interfacesHosts[42] = addressHosts.Assign(devicesHosts[42]);


	Ipv4AddressHelper addressRouters;
	vector<Ipv4InterfaceContainer> interfacesRouters(50);

	for (uint32_t i = 0; i < 50; i++)
	{
		ostringstream subset;
        	subset<<"140.2."<<i+1<<".0";
        	addressRouters.SetBase(subset.str().c_str (),"255.255.255.0");
		interfacesRouters[i] = addressRouters.Assign(devicesRouters[i]);
	}

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    NS_LOG_INFO("Create Application");

    //int temp;
    double curTime = 0.0;
    vector<ApplicationContainer> clientApps(1200);
    for(int i = 0; i<1200; i++)
    {
        randC(50);
        curTime += 0.1;
        hostPairs = 1+rand()%25;
        ApplicationContainer sinkApp;
        uint16_t port = 8080;

        for(int j = 0; j<hostPairs; j++)
        {
            Address sinkLocalAddress (InetSocketAddress (interfacesHosts[randlist[2*j+1]].GetAddress (0), port));
        PacketSinkHelper sinkHelper ("ns3::UdpSocketFactory", sinkLocalAddress);
            sinkApp.Add(sinkHelper.Install(nodeAdjacencyList[randlist[2*j+1]].Get(0)));
        }
        sinkApp.Start(Seconds(curTime-0.1));
        sinkApp.Stop (Seconds (curTime));
        OnOffHelper clientHelper ("ns3::UdpSocketFactory", Address ());
        clientHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.00016022]"));
        clientHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.00375]"));


       for(int j = 0; j<hostPairs; j++)
        {
            AddressValue remoteAddress
            (InetSocketAddress(interfacesHosts[randlist[2*j+1]].GetAddress(0),port));
            clientHelper.SetAttribute("Remote",remoteAddress);
            clientApps[i].Add(clientHelper.Install(nodeAdjacencyList[randlist[2*j]].Get(0)));
        }
        clientApps[i].Start(Seconds(curTime-0.1));
        clientApps[i].Stop (Seconds (curTime));
        for(int j = 0; j<hostPairs; j++)
        {
            clientApps[i].Get(j)->TraceConnectWithoutContext("Tx", MakeCallback(&sendCP));
            sinkApp.Get(j)->TraceConnectWithoutContext("Rx", MakeCallback(&configPacket));
        }
    }

    for(uint32_t i = 1; i < 120; i++)
	{
        //Simulator::Schedule(Seconds(i), &getmaxQ);
		Simulator::Schedule (Seconds (i), &calDelay);
		//Simulator::Schedule (Seconds (i), &calThroughput);
	}
    Simulator::Stop (Seconds (120));
	Simulator::Run();
	Simulator::Destroy ();

}

