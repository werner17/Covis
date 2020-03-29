#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <ctime>


struct	per_day
{
	unsigned	cases,
			deaths;

	double		cases_inc, deaths_inc;


	double	mortality() const
	{
		if (cases == 0) return 0.0;
		return double(deaths) /cases;
	}
};

struct	country
{
	unsigned			popData2018;
	std::map<unsigned,per_day>	evolution;

	unsigned	cases = 0, deaths = 0;

	double		cases_inc_3 = 0, deaths_inc_3 = 0,
			cases_inc_7 = 0, deaths_inc_7 = 0;

	unsigned	cases_10days_ago = 0;

	const	per_day&last_day() const
	{
		return evolution.rbegin()->second;
	}

	void	analyse()
	{
	int	last_day = evolution.rbegin()->first;

		for(auto&d : evolution) // note: increment could be more than a day, should consider this
		{
		auto&D = d.second;

			cases  += D.cases;
			deaths += D.deaths;

			D.cases_inc  =  cases==0?0: D.cases  / double(cases) ;
			D.deaths_inc = deaths==0?0: D.deaths / double(deaths) ;

			if ( d.first < last_day-10)
				cases_10days_ago += d.second.cases;
		}

	unsigned	days = 0;
		for(auto r = evolution.rbegin();
		    r != evolution.rend(); r++)
		{
		auto&D = r->second;
			if (days<4)
			{
				cases_inc_3  += D.cases_inc;
				deaths_inc_3 += D.deaths_inc;
			}

			if (days<8)
			{
				cases_inc_7  += D.cases_inc;
				deaths_inc_7 += D.deaths_inc;
			}
			else break;

			days++;
		}

		cases_inc_3  /= 3;
		deaths_inc_3 /= 3;
		cases_inc_7  /= 7;
		deaths_inc_7 /= 7;
	}
};


main()
{
std::ifstream in("edc.europa.eu.csv");
std::map<std::string, country> world;

std::string line;
	getline(in, line);
unsigned linenr = 0;
	while(getline(in, line))
	{
	//	0        1    2    3     4     5               6             7        8                  9
	//	dateRep,day,month,year,cases,deaths,countriesAndTerritories,geoId,countryterritoryCode,popData2018
	std::vector<std::string> fields;
		boost::split(fields, line, boost::is_any_of(","));

		if (fields[8] == "N/A") continue; // cruise ship

		linenr++;
	int	popData2018 = 0;
		try
		{
			popData2018 = std::stol(fields[9]);
		}
		catch(...)
		{
			std::cout << "Line Number " << linenr << ": "
				  <<  fields[8] << " -->" << fields[9] << std::endl;
			continue;
		}

	int	day    = std::stoi( fields[1] ),
		month  = std::stoi( fields[2] ),
		year   = std::stoi( fields[3] ),
		cases  = std::stoi( fields[4] ),
		deaths = std::stoi( fields[5] );

	country&C = world[ fields[8] ];
		C.popData2018 = popData2018;

	std::tm time = {};
		time. tm_mday = day;
		time. tm_mon  = month;
		time. tm_year = year-1900;
		mktime( &time);

	per_day&pd = C.evolution[ time.tm_yday ];
		pd.cases = cases;
		pd.deaths = deaths;
	}


	for(auto&C : world)
	{
//		std::cout << C.first << ",";
		C.second.analyse();

#if	0
		if (C.first == "AUT")
		{
			for(const auto&d : C.second.evolution)
				std::cout << d.first << " --> " << d.second.cases << std::endl;
		}
#endif
	}

std::ofstream out("result.csv");
	out << "Country,"
		"Cases,Daily Cases,Case Inc,3-day Case Inc,7-day Case inc,"
		"Diff Case %,Cases/1k,"
		"Deaths,Daily Deaths,Death Inc,3-day Death Inc,7-day Death inc,"
		"Diff Deaths %, Deaths/1k,"
		"Mortality,Delayed Mortality,Population 2018"
	    << std::endl;
	for(const auto&w : world)
	{
	const country&C = w.second;

		out
		    << w.first
		    << "," << C.cases
		    << "," << C.last_day().cases
		    << "," << C.last_day().cases_inc*100.0
		    << "," << C.cases_inc_3*100.0
		    << "," << C.cases_inc_7*100.0
		    << "," << (C.cases_inc_3-C.cases_inc_7)*100.0
		    << "," << 1000*double(C.cases) / C.popData2018

		    << ",\t" << C.deaths
		    << "," << C.last_day().deaths
		    << "," << C.last_day().deaths_inc*100.0
		    << "," << C.deaths_inc_3*100.0
		    << "," << C.deaths_inc_7*100.0
		    << "," << (C.deaths_inc_3-C.deaths_inc_7)*100.0
		    << "," << 1000*double(C.deaths) / C.popData2018

		    << ",\t" << C.last_day().mortality()*100
		    << "," << (C.cases_10days_ago>0?100*double(C.deaths) / C.cases_10days_ago:0)

		    << ",\t" << C.popData2018

		    << std::endl;
	}

	return 0;
}
