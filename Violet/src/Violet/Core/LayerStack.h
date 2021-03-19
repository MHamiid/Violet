#pragma once
#include "Layer.h"
namespace Violet {

	class VIOLET_API LayerStack {
		//LayerStack takes ownership of the Layers in term of deallocation
		//Push/Pop Overlay puts the specified layer after all the other layers
	public:
		LayerStack() = default;
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* overlayLayer);
		void popOverlay(Layer* overlayLayer);

		//std::vector iterators
		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }


	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};


}