/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef FORM_H
#define FORM_H

#include "ui_Form.h"

// VTK
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

// Qt
#include <QMainWindow>

class Form : public QMainWindow, public Ui::Form
{
  Q_OBJECT
public:

  // Constructor/Destructor
  Form();
  ~Form() {};

public slots:
  void on_actionOpen_activated();
  void on_actionSave_activated();

protected:

  vtkSmartPointer<vtkRenderer> Renderer;
  
  vtkSmartPointer<vtkPolyData> Contour;
  vtkSmartPointer<vtkPolyDataMapper> ContourMapper;
  vtkSmartPointer<vtkActor> ContourActor;
  // Create a cell array to store the lines in and add the lines to it
  vtkSmartPointer<vtkCellArray> Lines;
  
  std::vector<vtkSmartPointer<vtkActor> > PointActors;
  std::vector<vtkSmartPointer<vtkPolyDataMapper> > PointMappers;
  std::vector<vtkSmartPointer<vtkPolyData> > Points;
  
};

#endif // Form_H
