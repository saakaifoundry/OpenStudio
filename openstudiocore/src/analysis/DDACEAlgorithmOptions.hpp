/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ANALYSIS_DDACEALGORITHMOPTIONS_HPP
#define ANALYSIS_DDACEALGORITHMOPTIONS_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions.hpp"

#include "../utilities/core/Enum.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  class DDACEAlgorithmOptions_Impl;

} // detail

/** \class DDACEAlgorithmType 
 *  \brief Lists the types of sampling methods offered by the DDACE libary.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( DDACEAlgorithmType,
  ((central_composite)(Central Composite))
  ((box_behnken)(Box-Behnken))
  ((oas)(Orthogonal Array))
  ((grid)(Grid))
  ((random)(Monte Carlo))
  ((lhs)(Latin Hypercube))
  ((oa_lhs)(Orthogonal Array-Latin Hypercube))
);
 *  \endcode
 *  The valueName is the DAKOTA key for this algorithm type. The valueDescription is for 
 *  reading and display. */
OPENSTUDIO_ENUM( DDACEAlgorithmType,
  ((central_composite)(Central Composite))
  ((box_behnken)(Box-Behnken))
  ((oas)(Orthogonal Array))
  ((grid)(Grid))
  ((random)(Monte Carlo))
  ((lhs)(Latin Hypercube))
  ((oa_lhs)(Orthogonal Array-Latin Hypercube))
);

/** DDACEAlgorithmOptions is a DakotaAlgorithmOptions class that defines options specific to 
 *  DDACEAlgorithm. The user must always specify the DDACEAlgorithmType to use. Note that the
 *  central_composite, box_behnken, and grid methods require 
 *  problem.allVariablesAreContinuousOrStaticTransformations() == true, that is, it does not 
 *  make sense to send discrete variables to DAKOTA as fake continuous variables in these 
 *  cases. */
class ANALYSIS_API DDACEAlgorithmOptions : public DakotaAlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Sets the algorithmType, and also generates and sets a random seed. The default is to have 
   *  the seed fixed so that the analysis can be restarted in case of a failure. */
  explicit DDACEAlgorithmOptions(const DDACEAlgorithmType& algorithmType);

  /** Constructor provided for deserialization; not for general use. */
  DDACEAlgorithmOptions(const DDACEAlgorithmType& algorithmType,
                        const std::vector<Attribute>& options);

  virtual ~DDACEAlgorithmOptions() {}

  //@}

  /** Returns 1 + 2*n + 2^n, where n = problem.numContinuousVariables(). To be actually applicable,
   *  problem needs to have all discrete variable values specified (0-1 selected perturbations). */
  static int samplesForCentralComposite(const Problem& problem);

  /** Returns 1 + n * 2^(n-1), where n = problem.numContinuousVariables(). To be actually
   *  applicable, problem needs to have all discrete variable values specified (0-1 selected
   *  perturbations). */
  static int samplesForBoxBehnken(const Problem& problem);

  /** @name Getters */
  //@{

  /** Returns the type of DDACE sampling algorithm to be applied. */
  DDACEAlgorithmType algorithmType() const;

  /** Returns the explict pseudo-random number generator seed if it exists, evaluates to false 
   *  otherwise. */
  boost::optional<int> seed() const;

  /** Returns the user-specified number of samples to be used, if set; evaluates to false 
   *  otherwise. */
  boost::optional<int> samples() const;

  /** Returns the user-specified number of symbols to be used, if set; evaluates to false
   *  otherwise. Symbols is used by some of the algorithms to set the structure and number of
   *  samples, and can generally be calculated by DAKOTA if left unspecified. */
  boost::optional<int> symbols() const;

  //@}
  /** @name Setters */
  //@{

  void setSeed(int value);
  
  /** Clears the seed. Use with caution--clearing the seed value will essentially negate the 
   *  Dakota restart functionality, as the samples generated after restart will not match the 
   *  original samples. */
  void clearSeed();  

  bool setSamples(int value);
  
  void clearSamples();  

  bool setSymbols(int value);
  
  void clearSymbols();

  //@}
  /** @name Other */
  //@{
  
  // ETH@20120921 This text is a little unclear. Would be good to look at/add to the test cases to
  // figure out the true behavior.
  /** Equivalent to setSamples( numGridPartitions^(problem.numVariables()) ) and
   *  setSymbols(numGridPartitions). The grid sampling algorithm requires that samples be set, and
   *  determines the actual number of samples by taking numGridPartitions =
   *  ceil( samples ^ (1 / problem.numVariables()) ). */
  bool setSamplesForGrid(int numGridPartitions,const Problem& problem);

  /** Checks that symbols is prime or 4, sets symbols, and sets samples = multiplier * symbols^2.

   *  For use with Orthogonal Array and Orthogonal Array - Latin Hypercube. If dakota crashes after
   *  using this method, try calling clearSymbols afterwards and then look at the dakota stdout
   *  file to see what values dakota decides on for symbols and samples. Have had success with (2,1)
   *  and (2,2), but dakota crashed with (2,3). In general, the allowable samples and symbols 
   *  combinations seem to be more restricted than what the DAKOTA documentation indicates. */
  bool setSamplesAndSymbolsForOrthogonalArray(int symbols, int multiplier);
  
  //@}  
 protected:
  /// @cond
  typedef detail::DDACEAlgorithmOptions_Impl ImplType;

  explicit DDACEAlgorithmOptions(std::shared_ptr<detail::DDACEAlgorithmOptions_Impl> impl);

  friend class detail::DDACEAlgorithmOptions_Impl;
  friend class AlgorithmOptions;
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.DDACEAlgorithmOptions");
};

/** \relates DDACEAlgorithmOptions*/
typedef boost::optional<DDACEAlgorithmOptions> OptionalDDACEAlgorithmOptions;

/** \relates DDACEAlgorithmOptions*/
typedef std::vector<DDACEAlgorithmOptions> DDACEAlgorithmOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DDACEALGORITHMOPTIONS_HPP

