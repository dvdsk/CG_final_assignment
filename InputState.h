struct InputState
{
	//values for keys are in the form SDLK_a or SDLK_UP
	//keys that were pressed this frame
	bool keyspress[SDLK_LAST];
	//keys that are being held down
	bool keysdown[SDLK_LAST];
	
	int32_t mouse_rel_x;
	int32_t mouse_rel_y;
	bool mouse_moved;
}; 
