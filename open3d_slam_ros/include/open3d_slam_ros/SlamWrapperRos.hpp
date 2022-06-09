/*
 * SlamWrapperRos.hpp
 *
 *  Created on: Apr 19, 2022
 *      Author: jelavice
 */

#pragma once

#include <ros/ros.h>
#include <ros/package.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf2_ros/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

#include "open3d_slam/SlamWrapper.hpp"
#include "open3d_slam_msgs/SaveMap.h"
#include "open3d_slam_msgs/SaveSubmaps.h"

namespace o3d_slam {

class SlamWrapperRos: public SlamWrapper {

	using BASE = SlamWrapper;

public:
	SlamWrapperRos(ros::NodeHandlePtr nh);
	~SlamWrapperRos() override;

	bool saveMapCallback(open3d_slam_msgs::SaveMap::Request &req, open3d_slam_msgs::SaveMap::Response &res);
	bool saveSubmapsCallback(open3d_slam_msgs::SaveSubmaps::Request &req,
			open3d_slam_msgs::SaveSubmaps::Response &res);
	void loadParametersAndInitialize() override;
	void startWorkers() override;

private:

	void tfWorker();
	void visualizationWorker();

	void publishMaps(const Time &time);
	void publishDenseMap(const Time &time);
	void publishMapToOdomTf(const Time &time);

	ros::NodeHandlePtr nh_;
	std::shared_ptr<tf2_ros::TransformBroadcaster> tfBroadcaster_;
	ros::Publisher odometryInputPub_, mappingInputPub_, submapOriginsPub_, assembledMapPub_, denseMapPub_,
			submapsPub_;
	ros::ServiceServer saveMapSrv_, saveSubmapsSrv_;
	bool isVisualizationFirstTime_ = true;
	std::thread tfWorker_, visualizationWorker_;
	Time prevPublishedTimeScanToScan_, prevPublishedTimeScanToMap_;

};

} // namespace o3d_slam