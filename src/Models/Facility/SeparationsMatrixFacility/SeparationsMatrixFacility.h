// SeparationsMatrixFacility.h
#if !defined(_SEPARATIONSFACILITY_H)
#define _SEPARATIONSFACILITY_H

#include <iostream>
#include <queue>
#include <vector>

#include "Logger.h"
#include "FacilityModel.h"
#include "Material.h"

/**
   @class SeparationsMatrixFacility
   @brief This FacilityModel is still under construction.
   
   The SeparationsMatrixFacility class inherits from the FacilityModel 
   class and is dynamically loaded by the Model class when requested.
   
   @section intro Introduction 
   The SeparationsMatrixFacility is a facility type in Cyclus which accepts 
   spent fuel and separates it into multiple reusable material streams and at 
   least one waste stream. The transformation process from spent fuel to 
   streams is dictated by a separation matrix. A SeparationsMatrixFacility 
   offers the various types of separated material on their appropriate 
   markets. Shipments of these materials are executed when a market issues an 
   order that an offer has been matched with a request.
   
   @section modelparams Model Parameters 
   SeparationsMatrixFacility behavior is comprehensively defined by the 
   following parameters:
   - double capacity: The production capacity of the facility (TBD).
   - int startDate: The date on which the facility begins to operate (months).
   - int lifeTime: The length of time that the facility operates (months).
   - double inventorysize: the maximum quantity of material to be held in the 
   inventory
   - string name: A non-generic name for this facility.  
   - matrix sepMatrix: The separations matrix
   
   @section optionalparams Optional Parameters 
   SeparationsMatrixFacility behavior may also be specified with the following 
   optional parameters which have default values listed here.
   
   - double capacityFactor: The ratio of actual production capacity to the 
   rated production capacity. Default is 1 (actual/rated).
   - double availFactor: The percent of time the facility operates at its 
   capacity factor. Default is 100%.
   - double capitalCost: The cost of constructing and commissioning this 
   facility. Default is 0 ($).
   - double opCost: The annual cost of operation and maintenance of this 
   facility. Default is 0 ( $/year).
   - int constrTime: The number of months it takes to construct and commission 
   this facility. Default is 0 (months).
   - int decomTime: The number of months it takes to deconstruct and 
   decommission this facility. Default is 0 (months).
   - Inst* inst: The institution responsible for this facility.  double 
   price: the offer price per kg of processed fuel. Default is 0 ($).
   
   @section detailed Detailed Behavior 
   The SeparationsMatrixFacility starts operation when the simulation reaches 
   the month specified as the startDate. It immediately begins to request 
   usedFuel material. Each month it continues to request usedFuel at the rate 
   determined by its capacity and offers exactly as much separated material 
   as it has in its inventory. When it receives usedFuel that material is 
   placed into its stocks to await processing. The SeparationsMatrixFacility 
   then processes as much of the material in its stocks as is allowed by its 
   capacity. When it processes some usedFuel into separated streams it adds 
   that amount to its inventory. If an offer is matched with a request, the 
   SeparationsMatrixFacility executes that order by subtracting the quantity 
   from its inventory and sending that amount to the requesting facility. 
   When the simulation time equals the startDate plus the lifeTime, the 
   facility ceases to operate.  
  
   @section example Example Matrix 
   Here, describe how a separations matrix should be constructed and fed into 
   the simulation.
  
   @section issues Known Issues
   - <a href="http://code.google.com/p/cyclus/issues/detail?id=45">
   issue 45</a>: unclear behavior if user does not carefully account for 100% 
   of each element across all streams.
 */

typedef pair<std::string, mat_rsrc_ptr> InSep;
typedef pair<std::string, mat_rsrc_ptr> OutSep;

class SeparationsMatrixFacility : public FacilityModel  {
/* --------------------
 * all MODEL classes have these members
 * --------------------
 */
 public:
  /**
   * @brief Default constructor for SeparationsMatrixFacility Class
   */
  SeparationsMatrixFacility(){};

  /**
   * @brief every model should be destructable
   */
  virtual ~SeparationsMatrixFacility() {};
    
  /**
   * @brief every model needs a method to initialize from XML
   *
   * @param cur is the pointer to the model's xml node 
   */
  virtual void init(xmlNodePtr cur);
  
  /**
   * @brief every model needs a method to copy one object to another
   *
   * @param src is the StubStub to copy
   */
  virtual void copy(SeparationsMatrixFacility* src) ;

  /**
   * @brief This drills down the dependency tree to initialize all relevant 
   * parameters/containers.
   *
   * Note that this function must be defined only in the specific model in 
   * question and not in any inherited models preceding it.
   *
   * @param src the pointer to the original (initialized ?) model to be copied
   */
  virtual void copyFreshModel(Model* src);

  /**
   * @brief every model should be able to print a verbose description
   */
   virtual void print();

/* ------------------- */ 


/* --------------------
 * all COMMUNICATOR classes have these members
 * --------------------
 */
 public:
   /**
    * @brief When the facility receives a message, execute any transaction therein
    */
   virtual void receiveMessage(msg_ptr msg);
   
/* -------------------- */


/* --------------------
 * all FACILITYMODEL classes have these members
 * --------------------
 */
 public:
   /**
    * @brief The handleTick function specific to the SeparationsMatrixFacility.
    * At each tick, it requests as much raw inCommod as it can process this
    * month and offers as much outCommod as it will have in its inventory by
    * the
    * end of the month.
    *
    * @param time the time of the tick
    */
   virtual void handleTick(int time);

   /**
    * @brief The handleTick function specific to the SeparationsMatrixFacility.
    * At each tock, it processes material and handles orders, and records this
    * month's actions.
    *
    * @param time the time of the tock
    */
   virtual void handleTock(int time);

/* ------------------- */


/* --------------------
 * _THIS_ FACILITYMODEL class has these members
 * --------------------
 */
 protected:
   /**
    * @brief A typedef for the data structure representing a Facility's process
    * line, that is, the black box materials sit in while they're being
    * operated on.
    */
   typedef multimap<int, pair<msg_ptr, mat_rsrc_ptr> > ProcessLine;

   /**
    * @brief  Vector of incoming material
    */
   vector<std::string> in_commod_;

   /**
    * @brief  Vector of outgoing material
    */
   vector<std::string> out_commod_;

   /**
    * @brief The SeparationsMatrixFacility has a limit to how material it can process.
    * Units vary. It will be in the commodity unit per month.
    */
   double capacity_;

   /**
    * @brief This is the structure for each stream.
    */
   map<std::string,pair<int, double> > stream_set_;

   /**
    * @brief The stocks of raw material available to be processed.
    */
   deque<pair<std::string,mat_rsrc_ptr> > stocks_;
    
   /**
    * @brief The inventory of processed material.
    */
   deque<pair<std::string,mat_rsrc_ptr> > inventory_;

   /**
    * @brief The inventory of waste material.
    */
   deque<pair<std::string,mat_rsrc_ptr> > wastes_;

   /**
    * @brief The total mass flow required to process all outstanding orders this
    * Facility has already committed to. Units are tons, sometimes of uranium
    * and sometimes of certain isotopes. For Enrichment, they're tons U SWU.
    * For Fuel Fab, they're tons U.
    */
   double outstMF_;

   /**
    * @brief The list of orders to process on the Tock
    */
   deque<msg_ptr> ordersWaiting_;

   /**
    * @brief A map whose keys are times at which this Facility will finish executing a
    * given order and the values are pairs comprising the orders themselves and
    * the Materials each is to be made with.
    */
   ProcessLine ordersExecuting_;

   /**
    * @brief get the total mass of the stuff in the inventory
    *
    * @return the total mass of the materials in storage
    */
   double checkInventory();

   /**
    * @brief get the total mass of the stuff in the inventory
    *
    * @return the total mass of the materials in storage
    */
   double checkStocks();
    
   /**
    * @brief Separates all material waiting in the ordersExecuting ProcessLine
    */
   void separate();

   /**
    * @brief Makes requests according to how much spotCapacity is available.
    */
   void makeRequests();

   /**
    * @brief Makes offers according to how much spotCapacity is available.
    */
   void makeOffers();

   /**
    * @brief Checks to see if the given candidate Material can be used for separations
    * of any of the ordersWaiting for Material. If so, returns an iterator
    * pointing to that item. If not, returns an iterator just past the last
    * element.
    *
    * @param candMat the candidate material
    * @return the iterator
    */
   multimap<int,msg_ptr>::iterator checkOrdersWaiting(mat_rsrc_ptr
						      candMat);

   /**
    * @brief The time that the stock material spends in the facility.
    */
   int residence_time_;

   /**
    * @brief The maximum size that the inventory can grow to.
    * The SeparationsMatrixFacility must stop processing the material in its
    * stocks when its inventory is full.
    */
   int inventory_size_;

   /**
    * @brief Hack Force to Get Streams to operate by telling code how many streams to
    * separate
    */
   int nodeForce_;

   /* /\** */
   /*  * Stream of material in a list to use as Separations Guide */
   /*  * Contains matched set of element (integer) and sep fraction (double) */
   /*  *\/ */
   /* struct Stream; */

   /**
    * @brief The receipe of input materials.
    */
   mat_rsrc_ptr in_recipe_;

   /**
    * @brief The receipe of the output material.
    */
   mat_rsrc_ptr out_recipe_;
    
 public:
   /**
    * @brief Transacted resources are extracted through this method
    * 
    * @param order the msg/order for which resource(s) are to be prepared
    * @return list of resources to be sent for this order
    *
    */ 
   virtual std::vector<rsrc_ptr> removeResource(msg_ptr order);

   /**
    * @brief Transacted resources are received through this method
    *
    * @param trans the transaction to which these resource objects belong
    * @param manifest is the set of resources being received
    */ 
   virtual void addResource(msg_ptr msg,
			    std::vector<rsrc_ptr> manifest);

/* ------------------- */ 

};

#endif

