/*
                                                                                                                                                                     
g3data : A program for grabbing data from scanned graphs
Copyright (C) 2000 Jonas Frantz
                                                                                                                                                                     
    This file is part of g3data.
                                                                                                                                                                     
    g3data is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
                                                                                                                                                                     
    g3data is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                                                                                                                     
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
                                                                                                                                                                     
                                                                                                                                                                     
Authors email : jonas.frantz@welho.com
                                                                                                                                                                     
*/

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include "main.h"
#include "points.h"

/****************************************************************/
/* This function sets the numpoints entry to numpoints variable	*/
/* value.							*/
/****************************************************************/
void SetNumPointsEntry(GtkWidget *np_entry, gint np)
{
  char buf[128];

    sprintf(buf,"%d",np);
    gtk_entry_set_text(GTK_ENTRY(np_entry),buf);
}


/****************************************************************/
/* This function calculates the true value of the point based	*/
/* on the coordinates of the point on the bitmap.		*/
/****************************************************************/
struct PointValue CalcPointValue(gint Xpos,
                                 gint Ypos,
                                 gint control_point_image_coords[4][2],
                                 gdouble control_point_coords[4],
                                 gboolean logxy[2])
{
    double alpha,beta,x21,x43,y21,y43,rlc[4];				/* Declare help variables */
    struct PointValue PV;

    x21=(double) control_point_image_coords[1][0]-control_point_image_coords[0][0];			/* Calculate deltax of x axis points */
    y21=(double) control_point_image_coords[1][1]-control_point_image_coords[0][1];			/* Calculate deltay of x axis points */
    x43=(double) control_point_image_coords[3][0]-control_point_image_coords[2][0];			/* Calculate deltax of y axis points */
    y43=(double) control_point_image_coords[3][1]-control_point_image_coords[2][1];			/* Calculate deltay of y axis points */

    if (logxy[0] == TRUE) {							/* If x axis is logarithmic, store	*/
        rlc[0]=log(control_point_coords[0]);					/* recalculated values in rlc.		*/ 
        rlc[1]=log(control_point_coords[1]);
    }
    else {
        rlc[0]=control_point_coords[0];						/* Else store old values in rlc.	*/
        rlc[1]=control_point_coords[1];
    }

    if (logxy[1] == TRUE) {
        rlc[2]=log(control_point_coords[2]);					/* If y axis is logarithmic, store      */
        rlc[3]=log(control_point_coords[3]);					/* recalculated values in rlc.          */
    }
    else {
        rlc[2]=control_point_coords[2];						/* Else store old values in rlc.        */
        rlc[3]=control_point_coords[3];
    }

    alpha=((control_point_image_coords[0][0]-(double) Xpos)
          -(control_point_image_coords[0][1]-(double)  Ypos)*(x43/y43))/(x21-((y21*x43)/y43));
    beta=((control_point_image_coords[2][1]-(double) Ypos)
         -(control_point_image_coords[2][0]-(double) Xpos)*(y21/x21))/(y43-((x43*y21)/x21));

    if (logxy[0] == TRUE) PV.Xv = exp(-alpha*(rlc[1]-rlc[0])+rlc[0]);
    else PV.Xv = -alpha*(rlc[1]-rlc[0])+rlc[0];

    if (logxy[1] == TRUE) PV.Yv = exp(-beta*(rlc[3]-rlc[2])+rlc[2]);
    else PV.Yv = -beta*(rlc[3]-rlc[2])+rlc[2];

    alpha=((control_point_image_coords[0][0]-(double) (Xpos+1))
          -(control_point_image_coords[0][1]-(double) (Ypos+1))*(x43/y43))/(x21-((y21*x43)/y43));
    beta=((control_point_image_coords[2][1]-(double) (Ypos+1))
         -(control_point_image_coords[2][0]-(double) (Xpos+1))*(y21/x21))/(y43-((x43*y21)/x21));

    if (logxy[0] == TRUE) PV.Xerr = exp(-alpha*(rlc[1]-rlc[0])+rlc[0]);
    else PV.Xerr = -alpha*(rlc[1]-rlc[0])+rlc[0];

    if (logxy[1] == TRUE) PV.Yerr = exp(-beta*(rlc[3]-rlc[2])+rlc[2]);
    else PV.Yerr = -beta*(rlc[3]-rlc[2])+rlc[2];

    alpha=((control_point_image_coords[0][0]-(double) (Xpos-1))
          -(control_point_image_coords[0][1]-(double) (Ypos-1))*(x43/y43))/(x21-((y21*x43)/y43));
    beta=((control_point_image_coords[2][1]-(double) (Ypos-1))
         -(control_point_image_coords[2][0]-(double) (Xpos-1))*(y21/x21))/(y43-((x43*y21)/x21));

    if (logxy[0] == TRUE) PV.Xerr -= exp(-alpha*(rlc[1]-rlc[0])+rlc[0]);
    else PV.Xerr -= -alpha*(rlc[1]-rlc[0])+rlc[0];

    if (logxy[1] == TRUE) PV.Yerr -= exp(-beta*(rlc[3]-rlc[2])+rlc[2]);
    else PV.Yerr -= -beta*(rlc[3]-rlc[2])+rlc[2];

    PV.Xerr = fabs(PV.Xerr/4.0);
    PV.Yerr = fabs(PV.Yerr/4.0);

    return PV;
}
