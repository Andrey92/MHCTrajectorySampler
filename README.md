MHCTrajectorySampler
====================

MHCTrajectorySampler provides a set of likely trajectories of an object (called *RFID tag*) detected by a set of RFID readers located within a map.

Trajectories are generated with a Markov Chain Monte Carlo algorithm: a Metropolis Hastings modified in order to satisfy a set of constraints implied by the particular map topology. This specialized algorithm has been called Metropolis Hastings with Constraints [**MHC**].

Input
-----
Program input consists of:
* A map file, represented as a set of rectangular areas called *locations*, in which are located the RFID readers.
* A set of *constraints* relative to the specified map. There are three supported types of constraints:
	1. Direct Unreachability [**DU**] \(or Reachability [**DR**]) between two locations `L1`, `L2` (specifies whether `L2` is unreachable [or reachable] from `L1` in a single time unit);
	2. Traveling Time [**TT**] between two locations `L1`, `L2` (minimum number of time units needed to travel from `L1` to `L2`);
	3. Latency [**LT**] of a location `L` (minimum number of time units needed to consider `L` as 'visited').
* A set of readings. Each reading is a pair `<R,t>`, where `R` is the set of RFID readers which have detected the tag at instant `t` (`R` can be an empty set).

Output
------
MHCTrajectorySampler returns a set of likely trajectories of the RFID tag, useful to estimate the `P(L|R)` object, which represents the probability that the tag was in `L` while it was detected by the set `R` of readers.
