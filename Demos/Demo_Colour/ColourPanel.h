/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __COLOUR_PANEL_H__
#define __COLOUR_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class ColourPanel : public BaseLayout
{
public:
	ColourPanel();

	virtual void initialise();

	void show();
	void hide();

	inline bool isShow() { return mMainWidget->isShow(); } 

	void setColour(const Ogre::ColourValue & _colour);
	inline const Ogre::ColourValue & getColour() { return mCurrentColour; }

private:
	void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);
	void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
	void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _position);
	void notifyEditTextChange(MyGUI::WidgetPtr _sender);

	void updateFirst();
	void updateFromPoint(const MyGUI::IntPoint & _point);
	void updateFromColour(const Ogre::ColourValue & _colour);

private:
	MyGUI::WidgetPtr mColourRect;
	MyGUI::WidgetPtr mColourView;
	MyGUI::StaticImagePtr mImageColourPicker;
	MyGUI::EditPtr mEditRed;
	MyGUI::EditPtr mEditGreen;
	MyGUI::EditPtr mEditBlue;
	MyGUI::VScrollPtr mScrollRange;
	MyGUI::WidgetPtr mImageRange;

	Ogre::ColourValue mCurrentColour;
	Ogre::ColourValue mBaseColour;

	MyGUI::RawRect * mRawColourRect;
	MyGUI::RawRect * mRawColourView;

	std::vector<Ogre::ColourValue> mColourRange;
};

#endif // __COLOUR_PANEL_H__
