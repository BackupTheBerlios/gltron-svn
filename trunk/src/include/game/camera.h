#ifndef CAMERA_H
#define CAMERA_H

typedef struct Data Data;

typedef struct CameraType {
	int interpolated_cam;
	int interpolated_target;
	int coupled;
	int freedom[3];
	int type;
} CameraType;

typedef struct Camera {
	float cam[3];
	float target[3];
	float movement[4]; /* indices CAM_R, CAM_CHI, CAM_PHI, CAM_PHI_OFFSET */
	CameraType type;
} Camera;

typedef enum eCamType { 
	CAM_CIRCLE = 0,
	CAM_FOLLOW,
	CAM_COCKPIT,
	CAM_FREE
} eCamType;

enum { CAM_R = 0, CAM_CHI, CAM_PHI, CAM_PHI_OFFSET };

void initCamera(Camera *cam, Data *data, int type);
void nextCameraType(void);
void doCameraMovement(void);

#endif
