#!/bin/sh
export no_proxy=172.32.1.1
./vcaa_agent_client -gs "172.32.1.1:5001" -rtmp "rtmp://10.67.112.179:80/vcaa/huanlesong_full_720p.mp4" -deadline_ms 300000 -unload_ms 3000000