

| version         | Date   | Change History                                             |
| --------------- | ------ | ---------------------------------------------------------- |
| Initial version | ww39.2 | draft                                                      |
| v0.1            | ww39.4 | removed connection service. add working sequence/task list |
|                 |        |                                                            |
|                 |        |                                                            |



### VCAA Service Interface



`/* Video Analytics Service */`

VCAAVideoAnalyticsService {

​	`Status CreateVideoAnalyticsService(string model_name, AnalyticsConfigParameter param, AnalyticsServiceResponse *res)`

​	`Status DestroyVideoAnalyticsService(int service_id)`

​	`Status SetMediaStreamInfo(int service_id, string rtsp_addr)`

​	`Status StartInference(int service_id, InferenceResponse *res)`

​	`Status StopInference(int service_id)`

}

AnalyticsConfigParameter {

​	`ROI crop_settings[4]`

​	`int num_infer_request`

​	`int batch_size`

​	`string devices`

}

AnalyticsServiceResponse {

​	`int service_id`	

}

AnalyticsResult {

​	`// detections` 

​	`Detection detections`

​	`// raw video frame`

​	`RawFrame raw_frame`

}

InferenceResponse {

​	`AnalyticsResult infer_result` 

}

`/* Detection result */`

Detection {

​	`// label id`

​    `string label_id`

​    `// detect label`

​    `string label`

​    `// confidence`

​    `float confidence`

​    `// detected box`

​    `BoundingBox {`

​        `float xmin`

​        `float xmax`

​        `float ymin`

​        `float ymax`

​    `}`

​    `BoundingBox box`

}

RawFrame {

​	`int width`

​	`int height`

​	`string format`

​	`int64 timestamp`

​	`int size`

​	`DataBuffer buffer`

}

`/* Common return value */`

Status {

​	`FAILED = -1,`

​	`OK = 0,`

}



### Working sequence

- VCAA(server side)
  1. add listening port
  2. register service
  3. build and start servie
  4. wait _for_request
- E5(client side)
  1. create channel
  2. CreateVideoAnalyticsService
  3. SetMediaStreamInfo
  4. StartInferRequest
  5. HandleResponse



### Task List

| Task                                         | Effort Estimation | Owner | Comment                                                      |
| -------------------------------------------- | ----------------- | ----- | ------------------------------------------------------------ |
| 1. Project construction                      | 1w                |       |                                                              |
| 2. gRPC based VCAA service                   | 1.5w              |       | sample to demo C/S communication process                     |
| 3. Deep player, i.e. ffmpeg api based sample | 3w                |       | demux + decode + 4 tile inference + composition              |
| 4. Tracking                                  | 3w                |       | Integrate KCF solution from VPG, support multiple objects tracking.<br />1) OPENCV: CPU version KCF + multi tracker<br />2) GPU version KCF |

