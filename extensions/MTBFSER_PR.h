

#ifndef MTBFSER_PR_H
#define MTBFSER_PR_H

#include "ns3/ndnSIM/NFD/daemon/fw/strategy.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/process-nack-traits.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/retx-suppression-exponential.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"

#include <map>
#include <vector>

#include "utils/Interface.h"
#include "utils/MineTime.h"
#include "utils/PitQueue.h"
#include "utils/Srand.h"

#include <iostream>

using namespace std;

namespace nfd {
namespace fw  {

class MTBFSER_PR : public Strategy
                 , public ProcessNackTraits<MTBFSER_PR>
{
public :
  explicit
  MTBFSER_PR (Forwarder& forwarder, const Name& name = getStrategyName());

  static const Name&
  getStrategyName();

  void
  afterReceiveInterest(const Face& inFace, const Interest& interest,
                       const shared_ptr<pit::Entry>& pitEntry) override;

  void
  afterReceiveNack(const Face& inFace, const lp::Nack& nack,
                   const shared_ptr<pit::Entry>& pitEntry) override;

  void
  beforeSatisfyInterest ( const shared_ptr< pit::Entry > &pitEntry
                        , const Face &inFace, const Data &data) override;

  void
  afterContentStoreHit ( const shared_ptr< pit::Entry > &pitEntry
                       , const Face &inFace, const Data &data) override;

  void
  onDroppedInterest (const Face &outFace, const Interest & interest) override;

  void
  afterReceiveData (const shared_ptr <pit::Entry> &pitEntry,
                    const Face &inFace, const Data &data) override;

protected :

  bool
  IfAPeriod ();

  void
  DistributeTokenPeriodly ();

  void 
  CalculateWhenInterestHit ( const shared_ptr<pit::Entry> & pitEntry);

private :
  friend ProcessNackTraits<MTBFSER_PR>;
  RetxSuppressionExponential m_retxSuppression;

  //@brief The number interest packets which can be sent of this node in a period
  double NumberOfTokens;

  typedef std::map< const Face* , ns3::ndn::Interface > InterfaceList;
  InterfaceList interfaceList;

  typedef std::multimap < const Interest* , ns3::ndn::PitQueue > PitQueue;
  PitQueue pitQueue;

  void 
  TokenConsume (const Face* face);

  void
  TokenReward (const Data * data);

  ns3::ndn::MineTime Time;

  double perPeriodTime;

  double onePeriodTime;

  double numberOfPit;

  double LOWPIT;

  double UPPIT;

  // @brief Just for tipping now time
  double NOWTIME;

  double THRESHOLDV;

  double thresholdB;

PUBLIC_WITH_TESTS_ELSE_PRIVATE:
  static const time::milliseconds RETX_SUPPRESSION_INITIAL;
  static const time::milliseconds RETX_SUPPRESSION_MAX;

}; // class MTBFSER_PR

} // namespace fw
} // namespace nfd

#endif // MTBFSER_PR_H
