/***************************************************************************
                   kremoteview.h  -  widget that shows the remote framebuffer
                             -------------------
    begin                : Wed Dec 25 23:58:12 CET 2002
    copyright            : (C) 2002-2003 by Tim Jansen
    email                : tim@tjansen.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KREMOTEVIEW_H
#define KREMOTEVIEW_H


#include <qwidget.h>
#include <kkeynative.h>
#include "events.h"

typedef enum {
  QUALITY_UNKNOWN,
  QUALITY_HIGH,
  QUALITY_MEDIUM,
  QUALITY_LOW
} Quality;


/**
 * Generic widget that displays a remote framebuffer.
 */
class KRemoteView : public QWidget
{
	Q_OBJECT 
public:
	KRemoteView(QWidget *parent = 0, 
		    const char *name = 0, 
		    WFlags f = 0);

	virtual ~KRemoteView();

	/**
	 * Checks whether the backend supports scaling. The
	 * default implementation returns false.
	 * @return true if scaling is supported
	 * @see scaling()
	 */
	virtual bool supportsScaling();

	/**
	 * Checks whether the widget is in scale mode. The
	 * default implementation always returns false.
	 * @return true if scaling is activated. Must always be
	 *         false if @ref supportsScaling() returns false
	 * @see supportsScaling()
	 */
	virtual bool scaling();

	/**
	 * Returns the resolution of the remote framebuffer.
	 * It should return a null @ref QSize when the size 
	 * is not known.
	 * The backend must also emit a @ref changeSize()
	 * when the size of the framebuffer becomes available
	 * for the first time or the size changed.
	 * @return the remote framebuffer size, a null QSize
	 *         if unknown
	 */
	virtual QSize framebufferSize() = 0;

	/**
	 * Initiate the disconnection. This doesn't need to happen 
	 * immediately. The call must not block.
	 * @see isQuitting()
	 */
	virtual void startQuitting() = 0;

	/**
	 * Checks whether the view is currently quitting.
	 * @return true if it is quitting
	 * @see startQuitting()
	 */
	virtual bool isQuitting() = 0;

	/**
	 * Returns the host the view is connected to.
	 * @return the host the view is connected to
	 */
	virtual QString host() = 0;

	/**
	 * Returns the port the view is connected to.
	 * @return the port the view is connected to
	 */
	virtual int port() = 0;

	/**
	 * Starts connecting. Should not block.
	 * @return true if successful (so far), false
	 *         otherwise
	 * @see connected()
	 * @see disconnected()
	 * @see disconnectedError()
	 * @see statusChanged()
	 */
	virtual bool start() = 0;

	/**
	 * Returns the current status of the connection.
	 * @return the status of the connection
	 * @see setStatus()
	 */
	enum RemoteViewStatus status();
	
public slots:
        /**
	 * Called to enable or disable scaling.
	 * Ignored if @ref supportsScaling() is false.
	 * The default implementation does nothing.
	 * @param s true to enable, false to disable.
	 * @see supportsScaling()
	 * @see scaling()
	 */
        virtual void enableScaling(bool s);

        /**
	 * Sends a key to the remote server.
	 * @param k the key to send
	 */ 
        virtual void pressKey(XEvent *k) = 0; 

signals:
        /**
	 * Emitted when the size of the remote screen changes. Also
	 * called when the size is known for the first time.
	 * @param x the width of the screen
	 * @param y the height of the screen
	 */ 
	void changeSize(int w, int h);

	/**
	 * Emitted when the view connected successfully.
	 */
	void connected();

	/**
	 * Emitted when the view disconnected without error.
	 */
	void disconnected();

	/**
	 * Emitted when the view disconnected with error.
	 */
	void disconnectedError();

	/**
	 * Emitted when the status of the view changed.
	 * @param s the new status
	 */
	void statusChanged(RemoteViewStatus s);

	/**
	 * Emitted when the password dialog is shown or hidden.
	 * @param b true when the dialog is shown, false when it has 
	 *               been hidden
	 */
	void showingPasswordDialog(bool b);

	/**
	 * Emitted when the mouse on the remote side has been moved.
	 * @param x the new x coordinate
	 * @param y the new y coordinate
	 * @param buttonMask the mask of mouse buttons (bit 0 for first mouse
	 *                   button, 1 for second button etc)a
	 */
	void mouseStateChanged(int x, int y, int buttonMask);

protected:
	/**
	 * The status of the remote view.
	 * @return the status of the remote view
	 */
	enum RemoteViewStatus m_status;

	/**
	 * Set the status of the connection. 
	 * Emits a statusChanged() signal.
	 * Note that the states need to be set in a certain order,
	 * see @ref RemoteViewStatus. setStatus() will try to do this
	 * transition automatically, so if you are in REMOTE_VIEW_CONNECTING
	 * and call setStatus(REMOTE_VIEW_PREPARING), setStatus() will
	 * emit a REMOTE_VIEW_AUTHENTICATING and then REMOTE_VIEW_PREPARING.
	 * If you transition backwards, it will emit a
	 * REMOTE_VIEW_DISCONNECTED before doing the transition.
	 * @param s the new status
	 */
	virtual void setStatus(RemoteViewStatus s);
};

#endif
