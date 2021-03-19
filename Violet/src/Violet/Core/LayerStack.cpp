#include "VIOPCH.h"
#include "LayerStack.h"


namespace Violet {

	LayerStack::~LayerStack()
	{
		for (Layer* layer: m_layers) {
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		m_layerInsertIndex++;
		layer->onAttach();
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin()+ m_layerInsertIndex, layer); //search for layer
		if (it != m_layers.begin() + m_layerInsertIndex) {
			layer->onDetach();
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::pushOverlay(Layer* overlayLayer)
	{
		m_layers.emplace_back(overlayLayer);
		overlayLayer->onAttach();
	}

	void LayerStack::popOverlay(Layer* overlayLayer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlayLayer); //search for layer
		if (it != m_layers.end()) {
			overlayLayer->onDetach();
			m_layers.erase(it);
		}
	}

}