
#include "lib_Frames.h"
#include "geoparam_wgs84.h"
#include "Position.hpp"
#include "WGS84Ellipsoid.hpp"


namespace IMUSimulator {
	namespace Lib {


		Eigen::Vector3d transform_ecef2llh(Eigen::Vector3d& ecef) {

			Eigen::Vector3d llh;

			gpstk::WGS84Ellipsoid wgs84;
			gpstk::Position Coordinates(ecef[0],
										ecef[1],
										ecef[2],
										gpstk::Position::CoordinateSystem::Cartesian,
										&wgs84,
										gpstk::ReferenceFrame::WGS84);

			Coordinates.asGeodetic();

			llh[0] = Coordinates.geocentricLatitude();
			llh[1] = Coordinates.longitude();
			llh[2] = Coordinates.height();

			return llh;
		}

		void transform_ecef2llh(double& x, double& y, double& z, double& lat, double& lon, double& height) {

			Eigen::Vector3d ecef;
			Eigen::Vector3d llh;
			ecef << x,y,z;

			llh = transform_ecef2llh(ecef);

			lat = llh[0];
			lon = llh[1];
			height = llh[2];
		}

		Eigen::Vector3d transform_llh2ecef(Eigen::Vector3d& llh) {

			Eigen::Vector3d ecef;

			gpstk::WGS84Ellipsoid wgs84;
			gpstk::Position Coordinates(llh[0],
										llh[1],
										llh[2],
										gpstk::Position::CoordinateSystem::Geodetic,
										&wgs84,
										gpstk::ReferenceFrame::WGS84);

			Coordinates.asECEF();

			ecef[0] = Coordinates.getX();
			ecef[1] = Coordinates.getY();
			ecef[2] = Coordinates.getZ();

			return ecef;
		}

		void transform_llh2ecef(double& lat, double& lon, double& height, double& x, double& y, double& z) {
			
			Eigen::Vector3d ecef;
			Eigen::Vector3d llh;
			llh << lat, lon, height;

			ecef = transform_llh2ecef(llh);

			x = ecef[0];
			y = ecef[1];
			z = ecef[2];
		}

		Eigen::Matrix3d pos2Cne(double& lat, double& lon) {

			Eigen::Matrix3d Cne;

			double sL = std::sin(lat);
			double cL = std::cos(lat);
			double sl = std::sin(lon);
			double cl = std::cos(lon);

			Cne <<  -sL*cl, -sl, -cL*cl,
					-sL*sl,  cl, -cL*sl,
					 cL,     0,  -sL;

			return Cne;
		}

		Eigen::Matrix3d skew(Eigen::Vector3d& v) {
			Eigen::Matrix3d m;
			m <<	0,		-v[2],	 v[1],
					v[2],	 0,		-v[0],
				   -v[1],	 v[0],	 0;
			return m;
		}

		Eigen::Vector3d dcm2euler(Eigen::Matrix3d dcm) {

			Eigen::Vector3d eul;
			double pitch = std::asin(-dcm(2, 0));						// pitch is assumed to be[-pi pi].singular at pi.use ad - hoc methods to remedy this deficiency
			double roll = std::atan2(dcm(2, 1), dcm(2, 2));
			double heading = std::atan2(dcm(1, 0), dcm(0, 0));

			eul << roll, pitch, heading;

			return eul;
		}

		Eigen::Matrix3d euler2dcm(Eigen::Vector3d eul) {
				
			double cr = cos(eul(0)); double sr = sin(eul(0));	//roll
			double cp = cos(eul(1)); double sp = sin(eul(1));	//pitch
			double ch = cos(eul(2)); double sh = sin(eul(2));	//heading
			Eigen::Matrix3d	dcm = Eigen::Matrix3d::Zero();

			dcm(0, 0) = cp*ch;
			dcm(0, 1) = (sp*sr*ch) - (cr*sh);
			dcm(0, 2) = (cr*sp*ch) + (sh*sr);

			dcm(1, 0) = cp*sh;
			dcm(1, 1) = (sr*sp*sh) + (cr*ch);
			dcm(1, 2) = (cr*sp*sh) - (sr*ch);

			dcm(2, 0) = -sp;
			dcm(2, 1) = sr*cp;
			dcm(2, 2) = cr*cp;

			return dcm;
		}
	}
}